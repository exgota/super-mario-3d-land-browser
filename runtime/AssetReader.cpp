// Native asset inspection, separate from the ARMCC matching build.
// Public format descriptions and the owner's decrypted assets are the inputs.
// BYAML: https://nintendo-formats.com/libs/common/byaml.html
// Yaz0: https://wiki.cloudmodding.com/oot/Yaz_(File_Compression)
// NARC: https://loveemu.hatenablog.com/entry/20091002/nds_formats
// KCL: https://mkwiiki.org/wiki/KCL
// CGFX: https://www.3dbrew.org/wiki/CGFX

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace runtime {

using Bytes = std::span<const std::uint8_t>;
constexpr std::size_t MaximumAssetSize = 64 * 1024 * 1024;

class FormatError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class UnsupportedArchive : public FormatError {
public:
    using FormatError::FormatError;
};

void require(bool condition, const std::string& description) {
    if (!condition)
        throw FormatError(description);
}

// SHA-256 message schedule and compression from FIPS 180-4, sections 5 and 6.2.
// This verifies the complete owner executable before reading its data table.
std::string sha256(Bytes input) {
    constexpr std::array<std::uint32_t, 64> constants{
        0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
        0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
        0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
        0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
        0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
        0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
        0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
        0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};
    std::array<std::uint32_t, 8> state{
        0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
    require(input.size() <= MaximumAssetSize, "SHA-256 input exceeds its size limit");
    const std::size_t paddedSize = ((input.size() + 9 + 63) / 64) * 64;
    const std::uint64_t bitLength = std::uint64_t(input.size()) * 8;
    auto paddedByte = [&](std::size_t index) -> std::uint8_t {
        if (index < input.size()) return input[index];
        if (index == input.size()) return 0x80;
        if (index >= paddedSize - 8)
            return std::uint8_t(bitLength >> ((paddedSize - 1 - index) * 8));
        return 0;
    };
    for (std::size_t block = 0; block < paddedSize; block += 64) {
        std::array<std::uint32_t, 64> schedule{};
        for (std::size_t index = 0; index < 16; ++index)
            for (std::size_t byte = 0; byte < 4; ++byte)
                schedule[index] = (schedule[index] << 8) | paddedByte(block + index * 4 + byte);
        for (std::size_t index = 16; index < 64; ++index) {
            const std::uint32_t first = schedule[index - 15], second = schedule[index - 2];
            const std::uint32_t sigmaZero = std::rotr(first, 7) ^ std::rotr(first, 18) ^ (first >> 3);
            const std::uint32_t sigmaOne = std::rotr(second, 17) ^ std::rotr(second, 19) ^ (second >> 10);
            schedule[index] = schedule[index - 16] + sigmaZero + schedule[index - 7] + sigmaOne;
        }
        auto working = state;
        for (std::size_t index = 0; index < 64; ++index) {
            const auto [a, b, c, d, e, f, g, h] = working;
            const std::uint32_t sigmaOne = std::rotr(e, 6) ^ std::rotr(e, 11) ^ std::rotr(e, 25);
            const std::uint32_t choose = (e & f) ^ (~e & g);
            const std::uint32_t temporaryOne = h + sigmaOne + choose + constants[index] + schedule[index];
            const std::uint32_t sigmaZero = std::rotr(a, 2) ^ std::rotr(a, 13) ^ std::rotr(a, 22);
            const std::uint32_t majority = (a & b) ^ (a & c) ^ (b & c);
            working = {temporaryOne + sigmaZero + majority, a, b, c, d + temporaryOne, e, f, g};
        }
        for (std::size_t index = 0; index < state.size(); ++index)
            state[index] += working[index];
    }
    std::ostringstream output;
    output << std::hex << std::setfill('0');
    for (std::uint32_t word : state) output << std::setw(8) << word;
    return output.str();
}

std::size_t align(std::size_t value, std::size_t boundary) {
    return (value + boundary - 1) / boundary * boundary;
}

class ByteReader {
    Bytes mBytes;
    bool mBigEndian;

public:
    explicit ByteReader(Bytes bytes, bool bigEndian = false)
        : mBytes(bytes), mBigEndian(bigEndian) {}

    void check(std::size_t offset, std::size_t size) const {
        require(offset <= mBytes.size() && size <= mBytes.size() - offset,
                "Read extends outside its asset region");
    }

    Bytes bytes(std::size_t offset, std::size_t size) const {
        check(offset, size);
        return mBytes.subspan(offset, size);
    }

    std::uint8_t byte(std::size_t offset) const {
        check(offset, 1);
        return mBytes[offset];
    }

    std::uint32_t integer(std::size_t offset, std::size_t size = 4) const {
        require(size <= 4, "Integer exceeds 32 bits");
        check(offset, size);
        std::uint32_t result = 0;
        for (std::size_t index = 0; index < size; ++index) {
            const std::size_t shift = 8 * (mBigEndian ? size - 1 - index : index);
            result |= std::uint32_t(mBytes[offset + index]) << shift;
        }
        return result;
    }

    float floating(std::size_t offset) const {
        const float value = std::bit_cast<float>(integer(offset));
        require(std::isfinite(value), "Nonfinite asset float");
        return value;
    }

    bool magic(std::size_t offset, const std::string& expected) const {
        check(offset, expected.size());
        return std::equal(expected.begin(), expected.end(), mBytes.begin() + offset);
    }

    std::string text(std::size_t offset, std::size_t length) const {
        const Bytes content = bytes(offset, length);
        return std::string(reinterpret_cast<const char*>(content.data()), content.size());
    }

    std::string terminatedText(std::size_t offset, std::size_t limit) const {
        check(offset, limit);
        std::size_t length = 0;
        while (length < limit && mBytes[offset + length])
            ++length;
        require(length < limit, "Asset string has no terminator");
        return text(offset, length);
    }

    std::size_t relative(std::size_t field) const {
        const auto displacement = std::bit_cast<std::int32_t>(integer(field));
        const std::int64_t result = std::int64_t(field) + displacement;
        require(displacement != 0 && result >= 0 && std::uint64_t(result) < mBytes.size(),
                "Invalid self-relative asset pointer");
        return std::size_t(result);
    }
};

std::vector<std::uint8_t> readFile(const std::filesystem::path& path) {
    const auto size = std::filesystem::file_size(path);
    require(size <= MaximumAssetSize, "Asset exceeds the 64 MiB inspection limit");
    std::ifstream input(path, std::ios::binary);
    require(bool(input), "Cannot open asset");
    std::vector<std::uint8_t> result(static_cast<std::size_t>(size));
    if (size)
        input.read(reinterpret_cast<char*>(result.data()), static_cast<std::streamsize>(size));
    require(bool(input), "Asset ended during file read");
    return result;
}

std::vector<std::uint8_t> decompressYaz0(Bytes data) {
    ByteReader input(data, true);
    require(input.magic(0, "Yaz0"), "Expected Yaz0 compression");
    input.check(0, 16);
    const std::size_t size = input.integer(4);
    require(size <= MaximumAssetSize, "Decoded asset exceeds 64 MiB");
    std::vector<std::uint8_t> result;
    result.reserve(size);
    std::size_t position = 16;
    while (result.size() < size) {
        const std::uint8_t control = input.byte(position++);
        for (int bit = 7; bit >= 0 && result.size() < size; --bit) {
            if (control & (1 << bit)) {
                result.push_back(input.byte(position++));
            } else {
                const std::uint32_t pair = input.integer(position, 2);
                position += 2;
                const std::size_t distance = (pair & 0xFFF) + 1;
                std::size_t length = pair >> 12;
                length = length ? length + 2 : input.byte(position++) + 18;
                require(distance <= result.size() && length <= size - result.size(),
                        "Invalid Yaz0 back reference");
                for (std::size_t index = 0; index < length; ++index)
                    result.push_back(result[result.size() - distance]);
            }
        }
    }
    return result;
}

struct ArchiveMember {
    std::string name;
    Bytes content;
};

std::vector<ArchiveMember> readNarc(Bytes data) {
    ByteReader input(data);
    require(input.magic(0, "NARC") && input.integer(4, 2) == 0xFFFE &&
            input.integer(6, 2) == 0x100 && input.integer(8) == data.size() &&
            input.integer(12, 2) == 16 && input.integer(14, 2) == 3,
            "Unsupported or invalid NARC header");
    std::array<Bytes, 3> sections;
    const std::array<std::string, 3> expected = {"BTAF", "BTNF", "GMIF"};
    std::size_t position = 16;
    for (std::size_t number = 0; number < sections.size(); ++number) {
        require(input.magic(position, expected[number]), "Unexpected NARC section");
        const std::size_t size = input.integer(position + 4);
        require(size >= 8, "Invalid NARC section size");
        input.check(position, size);
        sections[number] = input.bytes(position + 8, size - 8);
        position += size;
    }
    require(position == data.size(), "NARC sections do not cover the archive");
    ByteReader allocation(sections[0]), names(sections[1]);
    const std::size_t count = allocation.integer(0);
    require(count <= (sections[0].size() - 4) / 8, "NARC allocation count exceeds its table");
    require(sections[0].size() == 4 + count * 8, "Invalid NARC allocation table length");
    const std::size_t nameOffset = names.integer(0);
    const std::uint32_t firstFile = names.integer(4, 2);
    const std::uint32_t directoryCount = names.integer(6, 2);
    require(directoryCount >= 1 && nameOffset >= 8 && nameOffset < sections[1].size(),
            "Invalid NARC directory table");
    if (directoryCount != 1)
        throw UnsupportedArchive("Nested NARC directory inspection is not implemented");
    require(nameOffset == 8 && firstFile == 0, "Unsupported flat NARC naming variant");
    std::vector<std::string> memberNames;
    std::set<std::string> uniqueNames;
    position = nameOffset;
    for (;;) {
        const std::uint8_t length = names.byte(position++);
        if (!length)
            break;
        require(!(length & 0x80), "Unexpected nested NARC name in a flat archive");
        const std::string name = names.text(position, length);
        require(name != "." && name != ".." && name.find_first_of("/\\") == std::string::npos &&
                name.find('\0') == std::string::npos && uniqueNames.insert(name).second,
                "Unsafe or duplicate NARC filename");
        memberNames.push_back(name);
        position += length;
    }
    require(memberNames.size() == count, "NARC filename count differs from allocation count");
    ByteReader payload(sections[2]);
    std::vector<ArchiveMember> result;
    std::size_t previousEnd = 0;
    for (std::size_t number = 0; number < count; ++number) {
        const std::size_t begin = allocation.integer(4 + number * 8);
        const std::size_t end = allocation.integer(8 + number * 8);
        require(end >= begin && begin >= previousEnd, "NARC members overlap or have reversed bounds");
        result.push_back({memberNames[number], payload.bytes(begin, end - begin)});
        previousEnd = end;
    }
    return result;
}

struct Value {
    using Array = std::vector<Value>;
    using Dictionary = std::map<std::string, Value>;
    using Storage = std::variant<std::monostate, bool, std::int32_t, std::uint32_t,
                                 float, std::string, Array, Dictionary>;
    Storage data;

    template <typename Type> const Type& get() const {
        const auto* result = std::get_if<Type>(&data);
        require(result != nullptr, "Unexpected BYAML value type");
        return *result;
    }
};

class ByamlReader {
    ByteReader mInput;
    std::uint32_t mVersion;
    std::vector<std::string> mKeys;
    std::vector<std::string> mStrings;
    std::set<std::size_t> mActive;
    std::map<std::size_t, Value> mCompleted;

    std::vector<std::string> readStrings(std::size_t offset) {
        if (!offset)
            return {};
        require(mInput.byte(offset) == 0xC2, "Invalid BYAML string table type");
        const std::size_t count = mInput.integer(offset + 1, 3);
        const std::size_t addressSize = 4 + (count + 1) * 4;
        mInput.check(offset, addressSize);
        std::vector<std::string> result;
        std::size_t begin = mInput.integer(offset + 4);
        require(begin >= addressSize, "BYAML strings overlap their address table");
        for (std::size_t number = 0; number < count; ++number) {
            const std::size_t end = mInput.integer(offset + 8 + number * 4);
            require(end > begin, "BYAML string offsets are not increasing");
            mInput.check(offset + begin, end - begin);
            const std::string value = mInput.terminatedText(offset + begin, end - begin);
            for (std::size_t padding = begin + value.size() + 1; padding < end; ++padding)
                require(mInput.byte(offset + padding) == 0, "Invalid BYAML string padding");
            require(result.empty() || result.back() < value, "BYAML strings are not sorted and unique");
            result.push_back(value); // Preserve encoded bytes, including CP932.
            begin = end;
        }
        mInput.check(offset + begin, 0);
        return result;
    }

    Value readValue(std::uint8_t type, std::uint32_t raw, std::size_t depth) {
        switch (type) {
        case 0xA0:
            require(raw < mStrings.size(), "BYAML string index is outside its table");
            return {mStrings[raw]};
        case 0xC0:
        case 0xC1:
            require(mInput.byte(raw) == type, "BYAML child container type differs from its reference");
            return readContainer(raw, depth + 1);
        case 0xD0:
            require(raw <= 1, "Invalid BYAML boolean");
            return {bool(raw)};
        case 0xD1:
            return {std::bit_cast<std::int32_t>(raw)};
        case 0xD2: {
            const float value = std::bit_cast<float>(raw);
            require(std::isfinite(value), "Nonfinite BYAML float");
            return {value};
        }
        case 0xD3:
            require(mVersion == 2, "Unsigned BYAML integer requires version 2");
            return {raw};
        case 0xFF:
            return {};
        default:
            throw FormatError("Unsupported BYAML node type");
        }
    }

    Value readContainer(std::size_t offset, std::size_t depth) {
        require(depth <= 256, "BYAML nesting exceeds the inspection limit");
        require(!mActive.contains(offset), "Cyclic BYAML graph");
        require(offset % 4 == 0, "Unaligned BYAML container");
        const std::uint8_t type = mInput.byte(offset);
        const std::size_t count = mInput.integer(offset + 1, 3);
        require(type == 0xC0 || type == 0xC1, "Invalid BYAML container type");
        if (mCompleted.contains(offset))
            return mCompleted.at(offset);
        mActive.insert(offset);
        Value result;
        if (type == 0xC0) {
            const std::size_t values = offset + 4 + align(count, 4);
            mInput.check(offset + 4, align(count, 4) + count * 4);
            Value::Array items;
            items.reserve(count);
            for (std::size_t number = 0; number < count; ++number)
                items.push_back(readValue(mInput.byte(offset + 4 + number),
                                          mInput.integer(values + number * 4), depth));
            result.data = std::move(items);
        } else {
            mInput.check(offset + 4, count * 8);
            Value::Dictionary items;
            std::uint32_t previous = 0;
            for (std::size_t number = 0; number < count; ++number) {
                const std::size_t position = offset + 4 + number * 8;
                const std::uint32_t key = mInput.integer(position, 3);
                require(key < mKeys.size() && (number == 0 || key > previous),
                        "BYAML dictionary keys are invalid or unsorted");
                items.emplace(mKeys[key], readValue(mInput.byte(position + 3),
                                                    mInput.integer(position + 4), depth));
                previous = key;
            }
            result.data = std::move(items);
        }
        mActive.erase(offset);
        mCompleted.emplace(offset, result);
        return result;
    }

public:
    explicit ByamlReader(Bytes data)
        : mInput(data, data.size() >= 2 && data[0] == 'B' && data[1] == 'Y'),
          mVersion(mInput.integer(2, 2)) {
        require(mInput.magic(0, "YB") || mInput.magic(0, "BY"), "Expected BYAML");
        require(mVersion == 1 || mVersion == 2, "Unsupported BYAML version");
        mInput.check(0, 16);
        mKeys = readStrings(mInput.integer(4));
        mStrings = readStrings(mInput.integer(8));
    }

    Value read() {
        const std::size_t root = mInput.integer(12);
        return root ? readContainer(root, 0) : Value{};
    }
};

using Vector3 = std::array<double, 3>;

Vector3 cross(const Vector3& first, const Vector3& second) {
    return {first[1] * second[2] - first[2] * second[1],
            first[2] * second[0] - first[0] * second[2],
            first[0] * second[1] - first[1] * second[0]};
}

double dot(const Vector3& first, const Vector3& second) {
    return first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
}

struct CollisionTriangle {
    std::uint32_t prismIndex;
    std::uint16_t attribute;
    std::array<Vector3, 3> positions;
};

struct CollisionMesh {
    std::size_t positionCount = 0;
    std::size_t normalCount = 0;
    std::size_t prismCapacity = 0;
    std::size_t nodeCount = 0;
    std::size_t leafCount = 0;
    std::size_t nonzeroLeafPrefixes = 0;
    double maximumPlaneResidual = 0;
    std::vector<CollisionTriangle> triangles;
};

CollisionMesh readKcl(Bytes data) {
    ByteReader input(data);
    input.check(0, 0x38);
    const std::size_t positions = input.integer(0);
    const std::size_t normals = input.integer(4);
    const std::size_t prismBase = input.integer(8);
    const std::size_t blocks = input.integer(12);
    require(prismBase <= data.size() - 16, "KCL prism base exceeds its file");
    require(positions == 0x38 && positions <= normals && normals <= prismBase + 16 &&
            prismBase + 16 <= blocks && blocks <= data.size(),
            "Unsupported ordered 3DS KCL section layout");
    require((normals - positions) % 12 == 0 && (prismBase + 16 - normals) % 12 == 0 &&
            (blocks - prismBase - 16) % 16 == 0,
            "Unaligned 3DS KCL tables");
    require(input.floating(16) > 0, "Invalid KCL prism thickness");
    for (std::size_t number = 0; number < 3; ++number)
        input.floating(20 + number * 4);
    CollisionMesh result;
    result.positionCount = (normals - positions) / 12;
    result.normalCount = (prismBase + 16 - normals) / 12;
    result.prismCapacity = (blocks - prismBase - 16) / 16;
    const std::uint32_t shift = input.integer(44);
    require(shift < 31, "Invalid KCL block width shift");
    std::array<std::uint64_t, 3> dimensions{};
    for (std::size_t number = 0; number < dimensions.size(); ++number) {
        const std::uint32_t width = ~input.integer(32 + number * 4);
        require(((std::uint64_t(width) + 1) & width) == 0, "Noncontiguous KCL area width mask");
        dimensions[number] = (width >> shift) + 1;
    }
    require(std::has_single_bit(dimensions[0]) && std::has_single_bit(dimensions[1]) &&
            std::has_single_bit(dimensions[2]), "Invalid KCL root dimensions");
    const std::uint32_t xShift = std::uint32_t(std::countr_zero(dimensions[0]));
    const std::uint32_t xyShift = xShift + std::uint32_t(std::countr_zero(dimensions[1]));
    require(input.integer(48) == xShift && input.integer(52) == xyShift,
            "KCL root indexing shifts differ from its dimensions");
    std::uint64_t rootCount = 1;
    for (const auto dimension : dimensions) {
        require(dimension <= data.size() / 4 / rootCount, "KCL root table exceeds its file");
        rootCount *= dimension;
    }
    input.check(blocks, std::size_t(rootCount) * 4);
    std::set<std::size_t> active, completed, leaves;
    std::set<std::uint32_t> referenced;
    std::function<void(std::size_t, std::size_t, std::size_t)> visit;
    visit = [&](std::size_t base, std::size_t count, std::size_t depth) {
        require(depth <= shift + 1, "KCL octree exceeds its subdivision depth");
        require(base >= blocks && base % 4 == 0 && !active.contains(base),
                "Invalid or cyclic KCL octree node");
        input.check(base, count * 4);
        if (completed.contains(base))
            return;
        active.insert(base);
        for (std::size_t number = 0; number < count; ++number) {
            const std::uint32_t descriptor = input.integer(base + number * 4);
            const std::size_t target = base + (descriptor & 0x7FFFFFFF);
            if (descriptor & 0x80000000) {
                require(target >= blocks && target % 2 == 0, "Invalid KCL leaf address");
                input.check(target, 2);
                if (!leaves.insert(target).second)
                    continue;
                if (input.integer(target, 2) != 0)
                    ++result.nonzeroLeafPrefixes;
                // In this 3DS variant the skipped prefix need not be zero.
                std::size_t position = target + 2;
                for (;;) {
                    const std::uint32_t identifier = input.integer(position, 2);
                    position += 2;
                    if (identifier == 0)
                        break;
                    require(identifier <= result.prismCapacity, "KCL leaf prism index is out of range");
                    referenced.insert(identifier);
                }
            } else {
                require(target != base, "KCL octree contains a self-reference");
                visit(target, 8, depth + 1);
            }
        }
        active.erase(base);
        completed.insert(base);
    };
    visit(blocks, std::size_t(rootCount), 0);
    result.nodeCount = completed.size();
    result.leafCount = leaves.size();
    auto vector = [&](std::size_t offset) -> Vector3 {
        return {input.floating(offset), input.floating(offset + 4), input.floating(offset + 8)};
    };
    for (std::size_t number = 0; number < result.positionCount; ++number)
        vector(positions + number * 12);
    for (std::size_t number = 0; number < result.normalCount; ++number)
        vector(normals + number * 12);
    for (std::size_t identifier = 1; identifier <= result.prismCapacity; ++identifier) {
        const std::size_t record = prismBase + identifier * 16;
        const double height = input.floating(record);
        const std::uint32_t positionIndex = input.integer(record + 4, 2);
        std::array<std::uint32_t, 4> normalIndices{};
        require(positionIndex < result.positionCount, "KCL position index is out of range");
        for (std::size_t number = 0; number < normalIndices.size(); ++number) {
            normalIndices[number] = input.integer(record + 6 + number * 2, 2);
            require(normalIndices[number] < result.normalCount, "KCL normal index is out of range");
        }
        if (!referenced.contains(std::uint32_t(identifier)))
            continue;
        const Vector3 position = vector(positions + positionIndex * 12);
        const Vector3 face = vector(normals + normalIndices[0] * 12);
        const Vector3 first = vector(normals + normalIndices[1] * 12);
        const Vector3 second = vector(normals + normalIndices[2] * 12);
        const Vector3 opposite = vector(normals + normalIndices[3] * 12);
        CollisionTriangle triangle{std::uint32_t(identifier), std::uint16_t(input.integer(record + 14, 2)),
                                   {position, {}, {}}};
        const std::array<Vector3, 2> edges = {cross(second, face), cross(first, face)};
        for (std::size_t number = 0; number < edges.size(); ++number) {
            const double denominator = dot(edges[number], opposite);
            require(std::isfinite(denominator) && std::abs(denominator) > 1e-10,
                    "Degenerate KCL prism edge");
            Vector3 displacement{};
            for (std::size_t axis = 0; axis < 3; ++axis) {
                displacement[axis] = edges[number][axis] * height / denominator;
                triangle.positions[number + 1][axis] = position[axis] + displacement[axis];
                require(std::isfinite(triangle.positions[number + 1][axis]), "Nonfinite reconstructed KCL vertex");
            }
            result.maximumPlaneResidual = std::max({result.maximumPlaneResidual,
                std::abs(dot(face, displacement)), std::abs(dot(opposite, displacement) - height)});
        }
        require(result.maximumPlaneResidual <= 1e-7, "Reconstructed KCL triangle violates its planes");
        result.triangles.push_back(triangle);
    }
    return result;
}

struct ResourceEntry {
    std::string name;
    std::size_t offset;
};

struct ResourceCategory {
    std::uint32_t index;
    std::vector<ResourceEntry> entries;
};

struct ModelAttribute {
    std::uint32_t semantic;
    std::uint32_t scalarType;
    std::uint32_t componentCount;
    std::uint32_t byteOffset;
    float multiplier;
    std::vector<std::uint32_t> rawComponents;
    std::vector<float> scaledComponents;
};

struct ModelVertexGroup {
    std::uint32_t flags;
    std::uint32_t semantic;
    std::string status;
    std::uint32_t stride;
    std::size_t vertexCount;
    std::vector<ModelAttribute> attributes;
    std::array<float, 4> inlineVectorFields;
};

struct ModelIndexStream {
    std::uint32_t scalarType;
    std::uint32_t primitiveField;
    std::string topology;
    std::size_t triangleCount;
    std::vector<std::uint32_t> indices;
};

struct ModelFaceGroup {
    std::uint32_t skinningField;
    std::vector<std::uint32_t> boneReferences;
    std::vector<ModelIndexStream> streams;
};

struct ModelShape {
    std::size_t offset;
    std::uint32_t flags;
    std::string status;
    std::array<float, 3> positionFields;
    std::vector<ModelVertexGroup> vertexGroups;
    std::vector<ModelFaceGroup> faceGroups;
};

struct ModelJoint {
    std::string name;
    std::size_t offset;
    std::uint32_t flags;
    std::uint32_t identifier;
    std::int32_t parentIdentifier;
    std::size_t parentOffset;
    std::array<float, 3> scale;
    std::array<float, 3> rotation;
    std::array<float, 3> translation;
    std::array<std::array<float, 12>, 3> matrixFields;
    std::uint32_t billboardField;
};

struct ModelSkeleton {
    std::size_t offset;
    std::uint32_t flags;
    std::string status;
    std::vector<ModelJoint> joints;
    std::uint32_t hierarchyMode = 0;
    std::uint32_t transformFlags = 0;
};

struct MaterialTextureReference {
    std::string status;
    std::int32_t mapperRelativePointerField = 0;
    std::size_t mapperOffset = 0;
    std::uint32_t mapperFlags = 0;
    std::size_t referenceOffset = 0;
    std::uint32_t referenceFlags = 0;
    bool referenceNamePresent = false;
    std::string referenceName;
    std::string targetName;
    std::int32_t cachedRelativePointerField = 0;
    std::size_t targetOffset = 0;
    std::uint32_t targetFlags = 0;
};

struct ModelMaterial {
    std::string name;
    std::size_t offset;
    std::uint32_t flags;
    std::string textureReferenceLayout;
    std::array<MaterialTextureReference, 3> textureReferences;
};

struct ModelMesh {
    std::string name;
    std::size_t offset;
    std::uint32_t flags;
    std::uint32_t shapeIndex;
    std::uint32_t materialIndex;
    std::size_t parentOffset;
    std::uint32_t drawFlagsField = 0;
};

struct ModelGeometry {
    std::string name;
    std::size_t offset;
    std::uint32_t flags;
    std::string status;
    std::vector<ModelShape> shapes;
    ModelSkeleton skeleton;
    std::string materialMappingStatus;
    std::vector<ModelMaterial> materials;
    std::vector<ModelMesh> meshes;
    std::array<float, 3> initializationScale{1, 1, 1};
    std::array<float, 3> initializationRotation{};
    std::array<float, 3> initializationTranslation{};
    std::array<float, 12> initializationMatrix84{};
};

struct CpuMeshBooleanTransition {
    static constexpr std::uint32_t ReplacementMask = 0x186;
    std::uint32_t setBits;

    std::uint32_t apply(std::uint32_t initialWord) const {
        return (initialWord & ~ReplacementMask) | setBits;
    }
};

// Original draw consumer 0x00191CE4 replaces four bits after material stages.
// These resource fields establish a transition, without establishing its input.
CpuMeshBooleanTransition meshBooleanTransition(const ModelMesh& mesh, const ModelShape& shape) {
    const bool modeTwo = std::any_of(shape.faceGroups.begin(), shape.faceGroups.end(),
                                    [](const ModelFaceGroup& face) { return face.skinningField == 2; });
    return {((mesh.drawFlagsField & 1) << 7) | ((mesh.drawFlagsField & 2) << 7) | (modeTwo ? 2u : 4u)};
}

struct CpuShaderContextInput {
    std::array<std::uint32_t, 6> vertexPacket{};
    std::array<std::uint32_t, 6> geometryPacket{};
    bool materialStatePresent = false;
    bool geometrySelectorPresent = false;
    std::int32_t geometrySelector = -1;
    std::uint8_t optionalCallbackFlag = 0;
};

struct CpuShaderContextTransfer {
    std::string status = "unavailable_caller_context";
    bool callerContextPresent = false;
    bool materialStatePresent = false;
    std::vector<std::uint32_t> commandWords;
    std::size_t cursorAdvanceBytes = 0;
};

// The original copies cached boolean/integer words through 0x002542BC.
// Constructor defaults cannot replace caller state modified by material code.
CpuShaderContextTransfer buildCpuShaderContextTransfer(const ModelMesh& mesh, const ModelShape& shape,
                                                       const CpuShaderContextInput* input) {
    CpuShaderContextTransfer result;
    if (!input)
        return result;
    result.callerContextPresent = true;
    result.materialStatePresent = input->materialStatePresent;
    if (!input->materialStatePresent) {
        result.status = "unavailable_material_context";
        return result;
    }
    if (!input->geometrySelectorPresent) {
        result.status = "unavailable_geometry_selector";
        return result;
    }
    if (input->optionalCallbackFlag) {
        result.status = "unsupported_context_callback";
        return result;
    }
    if (input->vertexPacket[1] != 0x804F02B0) {
        result.status = "unsupported_vertex_uniform_packet";
        return result;
    }
    const bool geometryPresent = input->geometrySelector >= 0;
    if (geometryPresent && input->geometryPacket[1] != 0x804F0280) {
        result.status = "unsupported_geometry_uniform_packet";
        return result;
    }
    result.commandWords.assign(input->vertexPacket.begin(), input->vertexPacket.end());
    result.commandWords[0] = meshBooleanTransition(mesh, shape).apply(result.commandWords[0]);
    if (geometryPresent)
        result.commandWords.insert(result.commandWords.end(), input->geometryPacket.begin(), input->geometryPacket.end());
    result.cursorAdvanceBytes = result.commandWords.size() * sizeof(std::uint32_t);
    result.status = "bounded_caller_owned_uniform_context_transfer";
    return result;
}

// The owner's revision uses OpenGL scalar enums in the documented CGFX records.
// These records retain resource-local data. Final vertex and material behavior stay open.
ModelGeometry readModelGeometry(Bytes data, std::size_t dataEnd, std::size_t imageStart,
                                std::uint32_t revision, const ResourceEntry& entry) {
    ByteReader input(data);
    auto metadata = [&](std::size_t offset, std::size_t size) {
        require(offset >= 20 && offset <= dataEnd && size <= dataEnd - offset,
                "CGFX model metadata escapes DATA");
    };
    std::size_t remainingComponents = data.size();
    auto consume = [&](std::size_t count) {
        require(count <= remainingComponents, "CGFX model exceeds its decoded component limit");
        remainingComponents -= count;
    };
    auto pointerList = [&](std::size_t field, std::size_t count) {
        metadata(field, 4);
        std::vector<std::size_t> result;
        if (!count) {
            require(input.integer(field) == 0, "Empty CGFX model list has a nonnull pointer");
            return result;
        }
        require(count <= (dataEnd - 20) / 4, "CGFX model list count exceeds DATA");
        const std::size_t list = input.relative(field);
        metadata(list, count * 4);
        consume(count);
        for (std::size_t index = 0; index < count; ++index) {
            const std::size_t object = input.relative(list + index * 4);
            metadata(object, 4);
            result.push_back(object);
        }
        return result;
    };
    auto imageBuffer = [&](std::size_t field, std::size_t size) {
        require(size != 0 && imageStart != 0, "CGFX model has an empty image buffer");
        const std::size_t offset = input.relative(field);
        require(offset >= imageStart && offset <= data.size() && size <= data.size() - offset,
                "CGFX model buffer escapes IMAG");
        return offset;
    };
    metadata(entry.offset, 8);
    require(input.magic(entry.offset + 4, "CMDL"), "CGFX model has no CMDL signature");
    ModelGeometry result{entry.name, entry.offset, input.integer(entry.offset), "resource_local_fields", {},
                         {0, 0, "not_present", {}}, "unresolved", {}, {}};
    if (revision != 0x05000000) {
        result.status = "unsupported_model_revision";
        return result;
    }
    metadata(entry.offset, 0xE0);
    // Base model constructor 0x00231660 consumes these serialized fields.
    for (std::size_t axis = 0; axis < 3; ++axis) {
        result.initializationScale[axis] = input.floating(entry.offset + 0x30 + axis * 4);
        result.initializationRotation[axis] = input.floating(entry.offset + 0x3C + axis * 4);
        result.initializationTranslation[axis] = input.floating(entry.offset + 0x48 + axis * 4);
    }
    for (std::size_t component = 0; component < 12; ++component)
        result.initializationMatrix84[component] = input.floating(entry.offset + 0x84 + component * 4);
    if (result.flags & 0x80) {
        metadata(entry.offset, 0xE4);
        const std::size_t skeleton = input.relative(entry.offset + 0xE0);
        metadata(skeleton, 8);
        require(input.magic(skeleton + 4, "SOBJ"), "CGFX skeleton has no SOBJ signature");
        result.skeleton = {skeleton, input.integer(skeleton), "unresolved_skeleton_layout", {}};
        if (result.skeleton.flags == 0x02000000) {
            metadata(skeleton, 0x2C);
            result.skeleton.status = "decoded_raw_joint_fields";
            result.skeleton.hierarchyMode = input.integer(skeleton + 0x24);
            result.skeleton.transformFlags = input.integer(skeleton + 0x28);
            const std::size_t count = input.integer(skeleton + 0x18);
            require(count <= (dataEnd - 20) / 0xE0, "CGFX joint count exceeds DATA");
            if (count) {
                const std::size_t dictionary = input.relative(skeleton + 0x1C);
                metadata(dictionary, 28 + count * 16);
                require(input.magic(dictionary, "DICT") && input.integer(dictionary + 8) == count &&
                        input.integer(dictionary + 4) >= 28 + count * 16 &&
                        input.integer(dictionary + 4) <= dataEnd - dictionary,
                        "Invalid CGFX joint dictionary");
                std::map<std::uint32_t, std::size_t> identifiers;
                std::set<std::size_t> offsets;
                std::set<std::string> names;
                consume(count * 45);
                for (std::size_t index = 0; index < count; ++index) {
                    const std::size_t node = dictionary + 28 + index * 16;
                    const std::size_t nameOffset = input.relative(node + 8);
                    metadata(nameOffset, 1);
                    const std::string name = input.terminatedText(nameOffset, dataEnd - nameOffset);
                    const std::size_t bone = input.relative(node + 12);
                    metadata(bone, 0xE0);
                    const std::size_t boneName = input.relative(bone);
                    metadata(boneName, 1);
                    require(!name.empty() && input.terminatedText(boneName, dataEnd - boneName) == name &&
                            names.insert(name).second && offsets.insert(bone).second,
                            "Invalid or duplicate CGFX joint name or record");
                    ModelJoint joint{name, bone, input.integer(bone + 4), input.integer(bone + 8),
                                     std::bit_cast<std::int32_t>(input.integer(bone + 0xC)), 0, {}, {}, {}, {},
                                     input.integer(bone + 0xD4)};
                    require(identifiers.emplace(joint.identifier, index).second, "Duplicate CGFX joint identifier");
                    if (input.integer(bone + 0x10)) {
                        joint.parentOffset = input.relative(bone + 0x10);
                        metadata(joint.parentOffset, 0xE0);
                    }
                    for (std::size_t axis = 0; axis < 3; ++axis) {
                        joint.scale[axis] = input.floating(bone + 0x20 + axis * 4);
                        joint.rotation[axis] = input.floating(bone + 0x2C + axis * 4);
                        joint.translation[axis] = input.floating(bone + 0x38 + axis * 4);
                    }
                    for (std::size_t matrix = 0; matrix < 3; ++matrix)
                        for (std::size_t component = 0; component < 12; ++component)
                            joint.matrixFields[matrix][component] = input.floating(bone + 0x44 + matrix * 0x30 + component * 4);
                    result.skeleton.joints.push_back(std::move(joint));
                }
                for (const ModelJoint& joint : result.skeleton.joints) {
                    if (joint.parentIdentifier == -1) {
                        require(joint.parentOffset == 0, "CGFX root joint has a parent pointer");
                    } else {
                        const auto parent = identifiers.find(std::uint32_t(joint.parentIdentifier));
                        require(joint.parentIdentifier >= 0 && parent != identifiers.end(), "CGFX joint parent identifier is missing");
                        require(joint.parentOffset == result.skeleton.joints[parent->second].offset,
                                "CGFX joint parent pointer disagrees with its identifier");
                    }
                }
                std::vector<std::uint8_t> visited(count, 0);
                for (std::size_t index = 0; index < count; ++index) {
                    std::vector<std::size_t> path;
                    std::size_t current = index;
                    while (!visited[current]) {
                        visited[current] = 1;
                        path.push_back(current);
                        const std::int32_t parent = result.skeleton.joints[current].parentIdentifier;
                        if (parent == -1)
                            break;
                        current = identifiers.at(std::uint32_t(parent));
                        require(visited[current] != 1, "CGFX joint hierarchy contains a cycle");
                    }
                    for (const std::size_t ancestor : path)
                        visited[ancestor] = 2;
                }
            } else {
                require(input.integer(skeleton + 0x1C) == 0, "Empty CGFX joint dictionary has a nonnull pointer");
            }
        }
    }
    for (const std::size_t shape : pointerList(entry.offset + 0xC8, input.integer(entry.offset + 0xC4))) {
        metadata(shape, 8);
        require(input.magic(shape + 4, "SOBJ"), "CGFX shape has no SOBJ signature");
        ModelShape record{shape, input.integer(shape), "resource_local_fields", {}, {}, {}};
        if (record.flags != 0x10000001) {
            record.status = "unsupported_shape_layout";
            result.shapes.push_back(std::move(record));
            continue;
        }
        metadata(shape, 0x40);
        for (std::size_t axis = 0; axis < 3; ++axis)
            record.positionFields[axis] = input.floating(shape + 0x20 + axis * 4);
        std::size_t vertexCount = 0;
        for (const std::size_t group : pointerList(shape + 0x3C, input.integer(shape + 0x38))) {
            metadata(group, 8);
            ModelVertexGroup vertices{input.integer(group), input.integer(group + 4),
                                      "unresolved_vertex_group_layout", 0, 0, {}, {}};
            if (vertices.flags == 0x80000000) {
                metadata(group, 0x30);
                vertices.status = "decoded_inline_vector_fields";
                for (std::size_t component = 0; component < 4; ++component)
                    vertices.inlineVectorFields[component] = input.floating(group + 0x20 + component * 4);
                record.vertexGroups.push_back(std::move(vertices));
                continue;
            }
            if (vertices.flags != 0x40000002) {
                record.vertexGroups.push_back(std::move(vertices));
                continue;
            }
            require(vertexCount == 0, "CGFX shape has multiple interleaved vertex groups");
            metadata(group, 0x30);
            const std::size_t size = input.integer(group + 0x14);
            vertices.stride = input.integer(group + 0x24);
            require(vertices.stride != 0 && size % vertices.stride == 0,
                    "Invalid CGFX vertex stride or buffer size");
            const std::size_t buffer = imageBuffer(group + 0x18, size);
            vertices.vertexCount = vertexCount = size / vertices.stride;
            vertices.status = "decoded_interleaved_attributes";
            std::set<std::uint32_t> semantics;
            for (const std::size_t declaration : pointerList(group + 0x2C, input.integer(group + 0x28))) {
                metadata(declaration, 0x34);
                require(input.integer(declaration) == 0x40000001, "Unsupported CGFX component declaration");
                ModelAttribute attribute{input.integer(declaration + 4), input.integer(declaration + 0x24),
                                         input.integer(declaration + 0x28), input.integer(declaration + 0x30),
                                         input.floating(declaration + 0x2C), {}, {}};
                require(semantics.insert(attribute.semantic).second, "Duplicate CGFX vertex semantic");
                std::size_t width;
                switch (attribute.scalarType) {
                    case 0x1400: case 0x1401: width = 1; break;
                    case 0x1402: case 0x1403: width = 2; break;
                    case 0x1406: width = 4; break;
                    default: throw FormatError("Unsupported CGFX component scalar type");
                }
                require(attribute.componentCount >= 1 && attribute.componentCount <= 4 &&
                        attribute.byteOffset <= vertices.stride &&
                        attribute.componentCount * width <= vertices.stride - attribute.byteOffset,
                        "CGFX component exceeds its vertex stride");
                consume(vertexCount * attribute.componentCount);
                for (std::size_t vertex = 0; vertex < vertexCount; ++vertex)
                    for (std::size_t component = 0; component < attribute.componentCount; ++component) {
                        const std::size_t offset = buffer + vertex * vertices.stride + attribute.byteOffset + component * width;
                        const std::uint32_t raw = input.integer(offset, width);
                        float value;
                        switch (attribute.scalarType) {
                            case 0x1400: value = std::bit_cast<std::int8_t>(std::uint8_t(raw)); break;
                            case 0x1402: value = std::bit_cast<std::int16_t>(std::uint16_t(raw)); break;
                            case 0x1406: value = input.floating(offset); break;
                            default: value = float(raw); break;
                        }
                        value *= attribute.multiplier;
                        require(std::isfinite(value), "Nonfinite scaled CGFX vertex component");
                        attribute.rawComponents.push_back(raw);
                        attribute.scaledComponents.push_back(value);
                    }
                vertices.attributes.push_back(std::move(attribute));
            }
            record.vertexGroups.push_back(std::move(vertices));
        }
        for (const std::size_t face : pointerList(shape + 0x30, input.integer(shape + 0x2C))) {
            metadata(face, 0x14);
            ModelFaceGroup faces{input.integer(face + 8), {}, {}};
            const std::size_t boneCount = input.integer(face);
            if (boneCount) {
                require(boneCount <= (dataEnd - 20) / 4, "CGFX bone reference count exceeds DATA");
                const std::size_t bones = input.relative(face + 4);
                metadata(bones, boneCount * 4);
                consume(boneCount);
                for (std::size_t index = 0; index < boneCount; ++index)
                    faces.boneReferences.push_back(input.integer(bones + index * 4));
            } else {
                require(input.integer(face + 4) == 0, "Empty CGFX bone list has a nonnull pointer");
            }
            for (const std::size_t primitive : pointerList(face + 0x10, input.integer(face + 0xC))) {
                metadata(primitive, 8);
                for (const std::size_t descriptor : pointerList(primitive + 4, input.integer(primitive))) {
                    metadata(descriptor, 0x10);
                    ModelIndexStream stream{input.integer(descriptor), input.integer(descriptor + 4), {}, 0, {}};
                    require(stream.scalarType == 0x1401 || stream.scalarType == 0x1403,
                            "Unsupported CGFX index scalar type");
                    const std::size_t width = stream.scalarType == 0x1401 ? 1 : 2;
                    const std::size_t size = input.integer(descriptor + 8);
                    require(size % width == 0, "CGFX index buffer has a partial scalar");
                    const std::size_t count = size / width;
                    // Retail 0x002B3940 maps only this byte through [4, 5, 6].
                    // Its caller can separately request geometry primitives.
                    const std::uint32_t mode = stream.primitiveField & 0xFF;
                    require(mode <= 2, "Unsupported CGFX serialized primitive mode");
                    if (mode == 0) {
                        require(count % 3 == 0, "CGFX triangle index count is not a multiple of three");
                        stream.topology = "triangles";
                        stream.triangleCount = count / 3;
                    } else {
                        require(count == 0 || count >= 3, "CGFX strip or fan has fewer than three indices");
                        stream.topology = mode == 1 ? "triangle_strip" : "triangle_fan";
                        stream.triangleCount = count == 0 ? 0 : count - 2;
                    }
                    const std::size_t buffer = imageBuffer(descriptor + 0xC, size);
                    consume(count);
                    for (std::size_t index = 0; index < count; ++index) {
                        const std::uint32_t value = input.integer(buffer + index * width, width);
                        require(vertexCount != 0 && value < vertexCount, "CGFX index exceeds its vertex count");
                        stream.indices.push_back(value);
                    }
                    faces.streams.push_back(std::move(stream));
                }
            }
            record.faceGroups.push_back(std::move(faces));
        }
        result.shapes.push_back(std::move(record));
    }
    const std::size_t materialCount = input.integer(entry.offset + 0xBC);
    require(materialCount <= (dataEnd - 20) / 16, "CGFX material count exceeds DATA");
    if (materialCount) {
        const std::size_t dictionary = input.relative(entry.offset + 0xC0);
        metadata(dictionary, 28 + materialCount * 16);
        require(input.magic(dictionary, "DICT") && input.integer(dictionary + 8) == materialCount &&
                input.integer(dictionary + 4) >= 28 + materialCount * 16 &&
                input.integer(dictionary + 4) <= dataEnd - dictionary,
                "Invalid CGFX material dictionary");
        std::set<std::string> names;
        std::set<std::size_t> offsets;
        consume(materialCount);
        for (std::size_t index = 0; index < materialCount; ++index) {
            const std::size_t node = dictionary + 28 + index * 16;
            const std::size_t nameOffset = input.relative(node + 8);
            metadata(nameOffset, 1);
            const std::string name = input.terminatedText(nameOffset, dataEnd - nameOffset);
            const std::size_t material = input.relative(node + 12);
            metadata(material, 0x10);
            require(input.magic(material + 4, "MTOB"), "CGFX material has no MTOB signature");
            const std::size_t materialName = input.relative(material + 0xC);
            metadata(materialName, 1);
            require(!name.empty() && input.terminatedText(materialName, dataEnd - materialName) == name &&
                    names.insert(name).second && offsets.insert(material).second,
                    "Invalid or duplicate CGFX material name or record");
            result.materials.push_back({name, material, input.integer(material), {}, {}});
        }
    } else {
        require(input.integer(entry.offset + 0xC0) == 0, "Empty CGFX material dictionary has a nonnull pointer");
    }
    std::set<std::size_t> meshOffsets;
    for (const std::size_t mesh : pointerList(entry.offset + 0xB8, input.integer(entry.offset + 0xB4))) {
        metadata(mesh, 0x24);
        metadata(mesh + 0x2C, sizeof(std::uint32_t));
        require(input.magic(mesh + 4, "SOBJ"), "CGFX mesh has no SOBJ signature");
        require(input.integer(mesh) == 0x01000000, "Unsupported CGFX mesh identity layout");
        require(meshOffsets.insert(mesh).second, "Duplicate CGFX mesh record");
        const std::size_t nameOffset = input.relative(mesh + 0xC);
        metadata(nameOffset, 1);
        ModelMesh record{input.terminatedText(nameOffset, dataEnd - nameOffset), mesh, input.integer(mesh),
                         input.integer(mesh + 0x18), input.integer(mesh + 0x1C), input.relative(mesh + 0x20),
                         input.integer(mesh + 0x2C)};
        // Retail 0x0033549C uses these two indices in the model's own lists.
        require(record.shapeIndex < result.shapes.size(), "CGFX mesh shape index exceeds its model");
        require(record.materialIndex < result.materials.size(), "CGFX mesh material index exceeds its model");
        require(record.parentOffset == entry.offset, "CGFX mesh parent disagrees with its model");
        result.meshes.push_back(std::move(record));
    }
    result.materialMappingStatus = "model_local_indices";
    return result;
}

struct TextureStorageTexel {
    std::uint32_t x;
    std::uint32_t y;
    std::size_t sourceOffset;
    std::uint16_t packedWord;
    std::array<std::uint8_t, 3> channels;
    std::array<std::uint8_t, 2> componentBytes{};
};

struct TextureStorageBlock {
    std::uint32_t x;
    std::uint32_t y;
    std::size_t sourceOffset;
    std::uint64_t packedWord;
    bool differential;
    bool flipped;
    bool definedEndpoints = true;
    std::array<std::uint8_t, 2> tableCodewords{};
    std::array<std::uint8_t, 3> endpointCodewords{};
    std::array<std::uint8_t, 3> secondaryCodewords{};
    std::array<std::int8_t, 3> signedDeltas{};
    std::array<std::uint8_t, 16> selectors{};
    bool hasAlpha = false;
    std::size_t alphaSourceOffset = 0;
    std::uint64_t alphaWord = 0;
    std::array<std::uint8_t, 16> alphaNibbles{};
};

struct TextureStorageLevel {
    std::uint32_t level;
    std::uint32_t width;
    std::uint32_t height;
    std::size_t serializedOffset;
    std::size_t byteCount;
    std::vector<TextureStorageTexel> texels;
    std::vector<TextureStorageBlock> blocks;
    std::vector<std::uint8_t> rgba8Pixels;
};

struct TextureImage {
    std::string name;
    std::size_t offset = 0;
    std::uint32_t flags = 0;
    std::string status;
    bool imageFieldsDecoded = false;
    std::uint32_t height = 0;
    std::uint32_t width = 0;
    std::uint32_t mipmapLevels = 0;
    std::uint32_t format = 0;
    std::size_t descriptorOffset = 0;
    std::uint32_t descriptorHeight = 0;
    std::uint32_t descriptorWidth = 0;
    std::uint32_t payloadByteCount = 0;
    std::size_t payloadOffset = 0;
    std::uint32_t cachedPointerField = 0;
    std::string storageDecodingStatus = "image_fields_unavailable";
    std::vector<TextureStorageLevel> storageLevels;
    std::string rgba8ReconstructionStatus = "storage_unavailable";
};

void reconstructTextureRgba8(TextureImage& texture) {
    if (texture.format == 6) {
        texture.rgba8ReconstructionStatus = "hilo8_semantics_unresolved";
        return;
    }
    for (const TextureStorageLevel& level : texture.storageLevels)
        for (const TextureStorageBlock& block : level.blocks)
            if (!block.definedEndpoints) {
                texture.rgba8ReconstructionStatus = "undefined_differential_endpoints";
                return;
            }
    // Khronos ETC1 extension 1.12 defines endpoint replication and these modifiers.
    constexpr int modifiers[8][4] = {
        {2, 8, -2, -8}, {5, 17, -5, -17}, {9, 29, -9, -29}, {13, 42, -13, -42},
        {18, 60, -18, -60}, {24, 80, -24, -80}, {33, 106, -33, -106}, {47, 183, -47, -183}
    };
    texture.rgba8ReconstructionStatus = texture.format == 3 ? "rgb565_normalized_nearest_rgba8" :
        texture.format == 12 ? "etc1_specification_rgba8" : "etc1_specification_alpha4_rgba8";
    for (TextureStorageLevel& level : texture.storageLevels) {
        const std::size_t pixelCount = std::size_t(level.width) * level.height;
        require(pixelCount <= std::numeric_limits<std::size_t>::max() / 4,
                "CGFX reconstructed RGBA8 extent overflows its byte count");
        level.rgba8Pixels.resize(pixelCount * 4);
        auto store = [&](std::uint32_t x, std::uint32_t y, const std::array<std::uint8_t, 4>& components) {
            const std::size_t offset = (std::size_t(y) * level.width + x) * 4;
            std::copy(components.begin(), components.end(), level.rgba8Pixels.begin() + offset);
        };
        if (texture.format == 3) {
            for (const TextureStorageTexel& texel : level.texels) {
                // Nearest RGBA8 representation of UNORM5/6, not a retail GPU quantization claim.
                const auto normalized = [](std::uint32_t value, std::uint32_t maximum) {
                    return std::uint8_t((value * 255 + maximum / 2) / maximum);
                };
                store(texel.x, texel.y, {normalized(texel.channels[0], 31), normalized(texel.channels[1], 63),
                                       normalized(texel.channels[2], 31), 255});
            }
            continue;
        }
        for (const TextureStorageBlock& block : level.blocks)
            for (std::uint32_t row = 0; row < 4; ++row)
                for (std::uint32_t column = 0; column < 4; ++column) {
                    const std::size_t position = row * 4 + column;
                    const std::size_t subblock = block.flipped ? row / 2 : column / 2;
                    const int modifier = modifiers[block.tableCodewords[subblock]][block.selectors[position]];
                    std::array<std::uint8_t, 4> components{};
                    for (std::size_t axis = 0; axis < 3; ++axis) {
                        const int codeword = subblock == 0 ? block.endpointCodewords[axis] : block.differential ?
                            block.endpointCodewords[axis] + block.signedDeltas[axis] : block.secondaryCodewords[axis];
                        const int expanded = block.differential ? (codeword << 3) | (codeword >> 2) :
                                                                 (codeword << 4) | codeword;
                        components[axis] = std::uint8_t(std::clamp(expanded + modifier, 0, 255));
                    }
                    components[3] = block.hasAlpha ? std::uint8_t(block.alphaNibbles[position] * 17) : 255;
                    store(block.x + column, block.y + row, components);
                }
    }
}

void readTextureStorage(Bytes data, TextureImage& texture) {
    texture.storageDecodingStatus = "unsupported_image_fields";
    if (texture.status != "resource_local_image_fields")
        return;
    texture.storageDecodingStatus = "unsupported_texture_format";
    if (texture.format != 3 && texture.format != 6 && texture.format != 12 && texture.format != 13)
        return;
    std::uint32_t width = texture.width;
    std::uint32_t height = texture.height;
    std::size_t remaining = texture.payloadByteCount;
    std::size_t offset = texture.payloadOffset;
    std::vector<TextureStorageLevel> levels;
    for (std::uint32_t level = 0; level < texture.mipmapLevels; ++level) {
        if (width < 8 || height < 8 || width % 8 || height % 8) {
            texture.storageDecodingStatus = level ? "unsupported_mipmap_tail" : "unsupported_storage_dimensions";
            return;
        }
        const std::size_t bytesPerTexelNumerator = texture.format == 3 || texture.format == 6 ? 2 : 1;
        const std::size_t bytesPerTexelDenominator = texture.format == 12 ? 2 : 1;
        require(height / bytesPerTexelDenominator <= remaining / bytesPerTexelNumerator / width,
                "CGFX texture storage mip level exceeds its image payload");
        const std::size_t byteCount = std::size_t(width) * (height / bytesPerTexelDenominator) * bytesPerTexelNumerator;
        levels.push_back({level, width, height, offset, byteCount, {}, {}, {}});
        offset += byteCount;
        remaining -= byteCount;
        width /= 2;
        height /= 2;
    }
    require(remaining == 0, "CGFX texture storage mip levels do not cover their image payload");
    ByteReader input(data);
    bool definedEndpoints = true;
    for (TextureStorageLevel& level : levels) {
        if (texture.format == 12 || texture.format == 13) {
            const std::size_t bytesPerBlock = texture.format == 13 ? 16 : 8;
            level.blocks.reserve(level.byteCount / bytesPerBlock);
            for (std::uint32_t y = 0; y < level.height; y += 4)
                for (std::uint32_t x = 0; x < level.width; x += 4) {
                    const std::size_t tile = std::size_t(y / 8) * (level.width / 8) + x / 8;
                    const std::size_t withinTile = ((y % 8) / 4) * 2 + (x % 8) / 4;
                    const std::size_t source = level.serializedOffset + (tile * 4 + withinTile) * bytesPerBlock +
                                               (texture.format == 13 ? 8 : 0);
                    const std::uint64_t packed = input.integer(source) | (std::uint64_t(input.integer(source + 4)) << 32);
                    TextureStorageBlock block{x, y, source, packed, bool((packed >> 33) & 1), bool((packed >> 32) & 1)};
                    if (texture.format == 13) {
                        block.hasAlpha = true;
                        block.alphaSourceOffset = source - 8;
                        block.alphaWord = input.integer(source - 8) | (std::uint64_t(input.integer(source - 4)) << 32);
                    }
                    block.tableCodewords = {std::uint8_t((packed >> 37) & 7), std::uint8_t((packed >> 34) & 7)};
                    for (std::size_t axis = 0; axis < 3; ++axis) {
                        const std::size_t shift = 56 - axis * 8;
                        block.endpointCodewords[axis] = std::uint8_t((packed >> (shift + (block.differential ? 3 : 4))) &
                                                                    (block.differential ? 31 : 15));
                        block.secondaryCodewords[axis] = std::uint8_t((packed >> shift) & (block.differential ? 7 : 15));
                        if (block.differential) {
                            const int encoded = block.secondaryCodewords[axis];
                            block.signedDeltas[axis] = std::int8_t(encoded < 4 ? encoded : encoded - 8);
                            const int endpoint = block.endpointCodewords[axis] + block.signedDeltas[axis];
                            block.definedEndpoints = block.definedEndpoints && endpoint >= 0 && endpoint <= 31;
                        }
                    }
                    for (std::uint32_t row = 0; row < 4; ++row)
                        for (std::uint32_t column = 0; column < 4; ++column) {
                            const std::uint32_t bit = column * 4 + row;
                            block.selectors[row * 4 + column] = std::uint8_t(((packed >> (bit + 16)) & 1) * 2 +
                                                                           ((packed >> bit) & 1));
                            if (block.hasAlpha)
                                block.alphaNibbles[row * 4 + column] = std::uint8_t((block.alphaWord >> (bit * 4)) & 15);
                        }
                    definedEndpoints = definedEndpoints && block.definedEndpoints;
                    level.blocks.push_back(std::move(block));
                }
            continue;
        }
        level.texels.reserve(level.byteCount / 2);
        for (std::uint32_t y = 0; y < level.height; ++y)
            for (std::uint32_t x = 0; x < level.width; ++x) {
                std::size_t withinTile = 0;
                for (std::uint32_t bit = 0; bit < 3; ++bit) {
                    withinTile |= std::size_t((x >> bit) & 1) << (2 * bit);
                    withinTile |= std::size_t((y >> bit) & 1) << (2 * bit + 1);
                }
                const std::size_t tile = std::size_t(y / 8) * (level.width / 8) + x / 8;
                const std::size_t source = level.serializedOffset + (tile * 64 + withinTile) * 2;
                const std::uint16_t packed = std::uint16_t(input.integer(source, 2));
                TextureStorageTexel texel{x, y, source, packed, {}};
                if (texture.format == 6)
                    texel.componentBytes = {std::uint8_t(packed & 255), std::uint8_t(packed >> 8)};
                else
                    texel.channels = {std::uint8_t((packed >> 11) & 31), std::uint8_t((packed >> 5) & 63),
                                      std::uint8_t(packed & 31)};
                level.texels.push_back(std::move(texel));
            }
    }
    texture.storageLevels = std::move(levels);
    texture.storageDecodingStatus = texture.format == 3 ? "rgb565_integer_storage" : texture.format == 6 ?
        "hilo8_raw_byte_storage" : texture.format == 12 ?
        (definedEndpoints ? "etc1_raw_block_storage" : "etc1_undefined_differential_endpoints") :
        (definedEndpoints ? "etc1a4_raw_block_storage" : "etc1a4_undefined_differential_endpoints");
    reconstructTextureRgba8(texture);
}

TextureImage readTextureImage(Bytes data, std::size_t dataEnd, std::size_t imageStart,
                              std::uint32_t revision, const ResourceEntry& entry) {
    ByteReader input(data);
    auto metadata = [&](std::size_t offset, std::size_t size) {
        require(offset >= 20 && offset <= dataEnd && size <= dataEnd - offset,
                "CGFX texture-image metadata escapes DATA");
    };
    metadata(entry.offset, 8);
    require(input.magic(entry.offset + 4, "TXOB"), "CGFX texture image has no TXOB signature");
    TextureImage result{};
    result.name = entry.name;
    result.offset = entry.offset;
    result.flags = input.integer(entry.offset);
    result.status = "unsupported_texture_revision";
    if (revision != 0x05000000)
        return result;
    result.status = result.flags == 0x20000004 ? "alias_texture_unresolved" : "unsupported_texture_layout";
    if (result.flags != 0x20000011)
        return result;
    metadata(entry.offset, 0x3C);
    const std::size_t nameOffset = input.relative(entry.offset + 0xC);
    metadata(nameOffset, 1);
    require(input.terminatedText(nameOffset, dataEnd - nameOffset) == entry.name,
            "CGFX texture name disagrees with its dictionary");
    result.height = input.integer(entry.offset + 0x18);
    result.width = input.integer(entry.offset + 0x1C);
    result.mipmapLevels = input.integer(entry.offset + 0x28);
    result.format = input.integer(entry.offset + 0x34);
    result.descriptorOffset = input.relative(entry.offset + 0x38);
    metadata(result.descriptorOffset, 0x1C);
    result.descriptorHeight = input.integer(result.descriptorOffset);
    result.descriptorWidth = input.integer(result.descriptorOffset + 4);
    result.payloadByteCount = input.integer(result.descriptorOffset + 8);
    result.cachedPointerField = input.integer(result.descriptorOffset + 0x18);
    require(result.payloadByteCount != 0 && imageStart != 0, "CGFX texture has an empty image payload");
    result.payloadOffset = input.relative(result.descriptorOffset + 0xC);
    require(result.payloadOffset >= imageStart && result.payloadOffset <= data.size() &&
            result.payloadByteCount <= data.size() - result.payloadOffset, "CGFX texture payload escapes IMAG");
    // Retail 0x002AC858 reads this descriptor and its serialized payload pointer.
    // Report the raw runtime cache without following it or translating addresses.
    result.imageFieldsDecoded = true;
    result.status = "resource_local_image_fields";
    if (result.height != result.descriptorHeight || result.width != result.descriptorWidth)
        result.status = "inconsistent_texture_dimensions";
    else if (!result.height || !result.width)
        result.status = "unsupported_texture_dimensions";
    else if (result.format != 3 && result.format != 6 && result.format != 12 && result.format != 13)
        result.status = "unsupported_texture_format";
    else if (!result.mipmapLevels)
        result.status = "unsupported_mipmap_count";
    readTextureStorage(data, result);
    return result;
}

struct ModelCatalog {
    std::uint32_t revision;
    std::vector<ResourceCategory> categories;
    std::vector<ModelGeometry> models;
    std::vector<TextureImage> textures;
};

void readMaterialTextureReferences(Bytes data, std::size_t dataEnd, ModelCatalog& catalog) {
    ByteReader input(data);
    auto metadata = [&](std::size_t offset, std::size_t size) {
        require(offset >= 20 && offset <= dataEnd && size <= dataEnd - offset,
                "CGFX texture-reference metadata escapes DATA");
    };
    auto encodedName = [&](std::size_t field) {
        const std::size_t offset = input.relative(field);
        metadata(offset, 1);
        return input.terminatedText(offset, dataEnd - offset);
    };
    std::map<std::string, std::size_t> textures;
    for (const ResourceCategory& category : catalog.categories)
        if (category.index == 1)
            for (const ResourceEntry& entry : category.entries)
                textures.emplace(entry.name, entry.offset);
    for (ModelGeometry& model : catalog.models)
        for (ModelMaterial& material : model.materials) {
            material.textureReferenceLayout = "unsupported_material_layout";
            if (material.flags != 0x08000000)
                continue;
            metadata(material.offset, 0x280);
            material.textureReferenceLayout = "observed_three_slot_layout";
            for (std::size_t slot = 0; slot < material.textureReferences.size(); ++slot) {
                MaterialTextureReference& record = material.textureReferences[slot];
                const std::size_t field = material.offset + 0x274 + slot * 4;
                record.mapperRelativePointerField = std::bit_cast<std::int32_t>(input.integer(field));
                record.status = "absent_mapper";
                if (!record.mapperRelativePointerField)
                    continue;
                record.mapperOffset = input.relative(field);
                metadata(record.mapperOffset, 4);
                record.mapperFlags = input.integer(record.mapperOffset);
                record.status = "unsupported_mapper_layout";
                if (record.mapperFlags != 0x80000000)
                    continue;
                metadata(record.mapperOffset, 0xC);
                record.status = "absent_reference";
                if (!input.integer(record.mapperOffset + 8))
                    continue;
                record.referenceOffset = input.relative(record.mapperOffset + 8);
                metadata(record.referenceOffset, 8);
                require(input.magic(record.referenceOffset + 4, "TXOB"),
                        "CGFX texture reference has no TXOB signature");
                record.referenceFlags = input.integer(record.referenceOffset);
                record.status = "unsupported_reference_layout";
                if (record.referenceFlags != 0x20000004)
                    continue;
                metadata(record.referenceOffset, 0x20);
                record.referenceNamePresent = input.integer(record.referenceOffset + 0xC) != 0;
                if (record.referenceNamePresent)
                    record.referenceName = encodedName(record.referenceOffset + 0xC);
                record.cachedRelativePointerField =
                    std::bit_cast<std::int32_t>(input.integer(record.referenceOffset + 0x1C));
                record.status = "absent_target_name";
                if (!input.integer(record.referenceOffset + 0x18))
                    continue;
                record.targetName = encodedName(record.referenceOffset + 0x18);
                record.status = "missing_local_target";
                const auto target = textures.find(record.targetName);
                if (target == textures.end())
                    continue;
                record.targetOffset = target->second;
                metadata(record.targetOffset, 8);
                require(input.magic(record.targetOffset + 4, "TXOB"),
                        "CGFX texture target has no TXOB signature");
                record.targetFlags = input.integer(record.targetOffset);
                // Retail 0x0022E73C resolves this name in the same CGFX dictionary.
                // Alias recursion and runtime cache application remain outside this reader.
                record.status = record.targetFlags == 0x20000011 ? "resolved_local_texture" :
                    record.targetFlags == 0x20000004 ? "alias_target_unresolved" : "unsupported_target_layout";
            }
        }
}

ModelCatalog readCgfx(Bytes data) {
    ByteReader input(data);
    require(input.magic(0, "CGFX") && input.byte(4) == 0xFF && input.byte(5) == 0xFE &&
            input.integer(6, 2) == 20 && input.integer(12) == data.size(),
            "Unsupported or invalid CGFX header");
    const std::size_t sectionCount = input.integer(16);
    require(sectionCount >= 1 && sectionCount <= 2, "Unsupported CGFX section count");
    std::size_t section = 20;
    std::size_t dataSize = 0;
    std::size_t imageStart = 0;
    for (std::size_t number = 0; number < sectionCount; ++number) {
        require(input.magic(section, number == 0 ? "DATA" : "IMAG"), "Unexpected CGFX section");
        const std::size_t size = input.integer(section + 4);
        require(size >= 8, "Invalid CGFX section size");
        input.check(section, size);
        if (number == 0)
            dataSize = size;
        else
            imageStart = section + 8;
        section += size;
    }
    require(section == data.size(), "CGFX sections do not cover their file");
    require(dataSize >= 8 + 16 * 8, "CGFX DATA catalog is truncated");
    ModelCatalog result{input.integer(8), {}, {}, {}};
    for (std::uint32_t number = 0; number < 16; ++number) {
        const std::size_t slot = 20 + 8 + number * 8;
        const std::size_t count = input.integer(slot);
        if (!count) {
            require(input.integer(slot + 4) == 0, "Empty CGFX catalog has a nonnull pointer");
            continue;
        }
        const std::size_t dictionary = input.relative(slot + 4);
        require(dictionary >= 20 && dictionary + 28 <= 20 + dataSize &&
                input.magic(dictionary, "DICT") && input.integer(dictionary + 8) == count,
                "Invalid CGFX dictionary header or entry count");
        const std::size_t dictionarySize = input.integer(dictionary + 4);
        require(count <= (dataSize - 28) / 16, "CGFX dictionary entry count exceeds DATA");
        require(dictionarySize >= 28 + count * 16 && dictionarySize <= 20 + dataSize - dictionary,
                "CGFX dictionary exceeds its DATA region");
        ResourceCategory category{number, {}};
        std::set<std::string> uniqueNames;
        for (std::size_t entry = 0; entry < count; ++entry) {
            const std::size_t record = dictionary + 28 + entry * 16;
            const std::size_t symbol = input.relative(record + 8);
            const std::size_t object = input.relative(record + 12);
            require(symbol >= 20 && symbol < 20 + dataSize && object >= 20 && object + 4 <= 20 + dataSize,
                    "CGFX resource pointer escapes its DATA region");
            const std::string name = input.terminatedText(symbol, 20 + dataSize - symbol);
            require(!name.empty() && uniqueNames.insert(name).second, "Empty or duplicate CGFX resource name");
            category.entries.push_back({name, object});
        }
        result.categories.push_back(std::move(category));
    }
    for (const ResourceCategory& category : result.categories)
        if (category.index == 0)
            for (const ResourceEntry& entry : category.entries)
                result.models.push_back(readModelGeometry(data, 20 + dataSize, imageStart, result.revision, entry));
    readMaterialTextureReferences(data, 20 + dataSize, result);
    for (const ResourceCategory& category : result.categories)
        if (category.index == 1)
            for (const ResourceEntry& entry : category.entries)
                result.textures.push_back(readTextureImage(data, 20 + dataSize, imageStart, result.revision, entry));
    return result;
}

// Resource-local identity from original loader 0x00167CAC and keeper 0x002B3340.
// This interface never applies serialized runtime pointers or constructs caller state.
struct ShaderInstanceDefinition {
    std::size_t offset = 0;
    std::uint32_t flags = 0;
    std::uint32_t vertexRootCacheField = 0;
    std::uint32_t geometryRootCacheField = 0;
    std::int32_t vertexSelector = -1;
    std::int32_t geometrySelector = -1;
    std::size_t vertexExecutableOffset = 0;
    std::size_t geometryExecutableOffset = 0;
    std::size_t parentOffset = 0;
    std::string status;
};

struct ShaderProgramDefinition {
    std::string name;
    std::size_t offset = 0;
    std::uint32_t flags = 0;
    std::uint32_t revision = 0;
    std::string status;
    std::vector<ShaderInstanceDefinition> instances;
};

struct ShaderArchiveSelectionInput {
    std::uint32_t revision = 0;
    std::string status;
    std::vector<ShaderProgramDefinition> programs;
};

ShaderArchiveSelectionInput readShaderArchiveSelectionInput(Bytes data) {
    const ModelCatalog catalog = readCgfx(data);
    ByteReader input(data);
    const std::size_t dataEnd = 20 + input.integer(24);
    auto metadata = [&](std::size_t offset, std::size_t size) {
        require(offset >= 20 && offset <= dataEnd && size <= dataEnd - offset,
                "CGFX shader selection metadata escapes DATA");
    };
    ShaderArchiveSelectionInput result{catalog.revision, "unsupported_shader_archive_revision", {}};
    if (catalog.revision != 0x05000000)
        return result;
    result.status = "resource_local_shader_catalog";
    for (const ResourceCategory& category : catalog.categories) {
        if (category.index != 4)
            continue;
        for (const ResourceEntry& entry : category.entries) {
            metadata(entry.offset, 0x10);
            require(input.magic(entry.offset + 4, "SHDR"), "CGFX shader program has no SHDR signature");
            ShaderProgramDefinition program{entry.name, entry.offset, input.integer(entry.offset),
                                            input.integer(entry.offset + 8), "unsupported_shader_program_layout", {}};
            if (program.flags != 0x80000002 || program.revision != 0x06000000) {
                result.programs.push_back(std::move(program));
                continue;
            }
            const std::size_t nameOffset = input.relative(entry.offset + 0xC);
            metadata(nameOffset, 1);
            require(input.terminatedText(nameOffset, dataEnd - nameOffset) == entry.name,
                    "CGFX shader program name disagrees with its dictionary");
            metadata(entry.offset, 0x48);
            program.status = "absent_executable_binary";
            if (!input.integer(entry.offset + 0x1C)) {
                result.programs.push_back(std::move(program));
                continue;
            }
            const std::size_t binary = input.relative(entry.offset + 0x1C);
            metadata(binary, 8);
            require(input.magic(binary, "DVLB"), "CGFX shader binary has no DVLB signature");
            const std::size_t executableCount = input.integer(binary + 4);
            require(executableCount <= (dataEnd - 20) / 4, "CGFX shader executable count exceeds DATA");
            metadata(binary + 8, executableCount * 4);
            program.status = "inconsistent_executable_count";
            if (!executableCount || input.integer(entry.offset + 0x20) != executableCount) {
                result.programs.push_back(std::move(program));
                continue;
            }
            std::vector<std::size_t> executables;
            for (std::size_t index = 0; index < executableCount; ++index) {
                const std::uint64_t executable = std::uint64_t(binary) + input.integer(binary + 8 + index * 4);
                require(executable <= dataEnd, "CGFX executable offset escapes DATA");
                metadata(std::size_t(executable), 8);
                require(input.magic(std::size_t(executable), "DVLE"), "CGFX executable has no DVLE signature");
                executables.push_back(std::size_t(executable));
            }
            const std::size_t count = input.integer(entry.offset + 0x28);
            require(count <= (dataEnd - 20) / 4, "CGFX shader instance count exceeds DATA");
            program.status = "absent_shader_instances";
            if (!count) {
                require(input.integer(entry.offset + 0x2C) == 0, "Empty CGFX shader instance list has a nonnull pointer");
                result.programs.push_back(std::move(program));
                continue;
            }
            const std::size_t list = input.relative(entry.offset + 0x2C);
            metadata(list, count * 4);
            std::set<std::size_t> uniqueInstances;
            for (std::size_t index = 0; index < count; ++index) {
                const std::size_t offset = input.relative(list + index * 4);
                metadata(offset, 4);
                require(uniqueInstances.insert(offset).second, "Duplicate CGFX shader instance");
                if (input.integer(offset) != 3) {
                    ShaderInstanceDefinition instance;
                    instance.offset = offset;
                    instance.flags = input.integer(offset);
                    instance.status = "unsupported_shader_instance_layout";
                    program.instances.push_back(std::move(instance));
                    continue;
                }
                metadata(offset, 0x88);
                ShaderInstanceDefinition instance{offset, input.integer(offset), input.integer(offset + 4),
                    input.integer(offset + 8), std::bit_cast<std::int32_t>(input.integer(offset + 0xC)),
                    std::bit_cast<std::int32_t>(input.integer(offset + 0x10)), 0, 0,
                    input.relative(offset + 0x84), "invalid_executable_selector"};
                require(instance.parentOffset == entry.offset, "CGFX shader instance parent disagrees with its root");
                if (instance.vertexSelector >= 0 && std::uint32_t(instance.vertexSelector) < executableCount &&
                    (instance.geometrySelector < 0 || std::uint32_t(instance.geometrySelector) < executableCount)) {
                    instance.vertexExecutableOffset = executables[std::uint32_t(instance.vertexSelector)];
                    if (instance.geometrySelector >= 0)
                        instance.geometryExecutableOffset = executables[std::uint32_t(instance.geometrySelector)];
                    instance.status = "resource_local_instance_identity";
                }
                program.instances.push_back(std::move(instance));
            }
            program.status = "resource_local_program_identity";
            result.programs.push_back(std::move(program));
        }
    }
    return result;
}

struct MaterialShaderSelection {
    std::size_t modelOffset = 0;
    std::size_t materialOffset = 0;
    bool optionalShaderArchivePresent = false;
    std::string status;
    std::int32_t referenceRelativePointerField = 0;
    std::size_t referenceOffset = 0;
    std::uint32_t referenceFlags = 0;
    bool referenceNamePresent = false;
    std::string referenceName;
    std::int32_t cachedRelativePointerField = 0;
    std::uint32_t instanceIndex = 0;
    bool instanceSelected = false;
    std::string selectedProgramName;
    std::size_t selectedProgramOffset = 0;
    std::size_t selectedInstanceOffset = 0;
    std::int32_t vertexSelector = -1;
    std::int32_t geometrySelector = -1;
    std::size_t vertexExecutableOffset = 0;
    std::size_t geometryExecutableOffset = 0;
    bool sameAsNamedInitializerInstance = false;
};

// The optional archive is an explicit original loader input, never an inferred default.
// Nonzero serialized caches are preserved and unapplied. Runtime addresses are not file offsets.
std::vector<MaterialShaderSelection> readMaterialShaderSelections(Bytes data, const ModelCatalog& catalog,
                                                                const ShaderArchiveSelectionInput* shaderArchive) {
    ByteReader input(data);
    const std::size_t dataEnd = 20 + input.integer(24);
    auto metadata = [&](std::size_t offset, std::size_t size) {
        require(offset >= 20 && offset <= dataEnd && size <= dataEnd - offset,
                "CGFX material shader metadata escapes DATA");
    };
    std::vector<MaterialShaderSelection> result;
    for (std::size_t modelIndex = 0; modelIndex < catalog.models.size(); ++modelIndex) {
        const ModelGeometry& model = catalog.models[modelIndex];
        for (const ModelMaterial& material : model.materials) {
            MaterialShaderSelection selection;
            selection.modelOffset = model.offset;
            selection.materialOffset = material.offset;
            selection.optionalShaderArchivePresent = shaderArchive != nullptr;
            selection.status = "unsupported_material_shader_layout";
            if (catalog.revision != 0x05000000 || material.flags != 0x08000000) {
                result.push_back(std::move(selection));
                continue;
            }
            metadata(material.offset, 0x290);
            selection.instanceIndex = input.integer(material.offset + 0x28C);
            selection.referenceRelativePointerField = std::bit_cast<std::int32_t>(input.integer(material.offset + 0x284));
            selection.status = "absent_material_shader_reference";
            if (!selection.referenceRelativePointerField) {
                result.push_back(std::move(selection));
                continue;
            }
            selection.referenceOffset = input.relative(material.offset + 0x284);
            metadata(selection.referenceOffset, 8);
            require(input.magic(selection.referenceOffset + 4, "SHDR"), "CGFX material shader reference has no SHDR signature");
            selection.referenceFlags = input.integer(selection.referenceOffset);
            selection.status = "unsupported_material_shader_reference";
            if (selection.referenceFlags != 0x80000001) {
                result.push_back(std::move(selection));
                continue;
            }
            metadata(selection.referenceOffset, 0x20);
            selection.referenceNamePresent = input.integer(selection.referenceOffset + 0x18) != 0;
            if (selection.referenceNamePresent) {
                const std::size_t nameOffset = input.relative(selection.referenceOffset + 0x18);
                metadata(nameOffset, 1);
                selection.referenceName = input.terminatedText(nameOffset, dataEnd - nameOffset);
            }
            selection.cachedRelativePointerField = std::bit_cast<std::int32_t>(input.integer(selection.referenceOffset + 0x1C));
            selection.status = "serialized_shader_cache_unapplied";
            if (selection.cachedRelativePointerField) {
                result.push_back(std::move(selection));
                continue;
            }
            selection.status = "unavailable_optional_shader_archive";
            if (!shaderArchive) {
                result.push_back(std::move(selection));
                continue;
            }
            selection.status = "unsupported_secondary_model_fixup";
            if (modelIndex != 0) {
                result.push_back(std::move(selection));
                continue;
            }
            selection.status = shaderArchive->status;
            if (shaderArchive->status != "resource_local_shader_catalog") {
                result.push_back(std::move(selection));
                continue;
            }
            const auto program = std::find_if(shaderArchive->programs.begin(), shaderArchive->programs.end(),
                [](const ShaderProgramDefinition& entry) { return entry.name == "FastShader"; });
            selection.status = "missing_fast_shader_root";
            if (program == shaderArchive->programs.end()) {
                result.push_back(std::move(selection));
                continue;
            }
            selection.status = program->status;
            if (program->status != "resource_local_program_identity") {
                result.push_back(std::move(selection));
                continue;
            }
            selection.selectedProgramName = program->name;
            selection.selectedProgramOffset = program->offset;
            selection.status = "invalid_shader_instance_index";
            if (selection.instanceIndex >= program->instances.size()) {
                result.push_back(std::move(selection));
                continue;
            }
            const ShaderInstanceDefinition& instance = program->instances[selection.instanceIndex];
            selection.instanceSelected = true;
            selection.selectedInstanceOffset = instance.offset;
            selection.vertexSelector = instance.vertexSelector;
            selection.geometrySelector = instance.geometrySelector;
            selection.vertexExecutableOffset = instance.vertexExecutableOffset;
            selection.geometryExecutableOffset = instance.geometryExecutableOffset;
            selection.sameAsNamedInitializerInstance = selection.instanceIndex == 0;
            selection.status = instance.status == "resource_local_instance_identity" ?
                "resource_local_optional_archive_shader_selection" : instance.status;
            result.push_back(std::move(selection));
        }
    }
    return result;
}

struct VertexUniformCoefficient {
    std::size_t sourceOffset = 0;
    std::uint32_t semantic = 0;
    std::uint8_t inputRegisterByte = 0;
    bool constant = false;
    std::uint8_t componentCount = 0;
    std::uint32_t multiplierWord = 0;
};

struct CpuVertexUniformConstruction {
    std::string status;
    bool shaderSelectionPresent = false;
    std::size_t meshOffset = 0;
    std::size_t shapeOffset = 0;
    std::size_t shaderInstanceOffset = 0;
    std::uint32_t initialMeshFlags = 0;
    std::uint32_t constructedMeshFlags = 0;
    std::vector<VertexUniformCoefficient> coefficients;
    std::vector<std::uint32_t> commandWords;
};

// Retail 0x002AD654 constructs this transfer; 0x00190FE8 copies its 48 bytes.
// Coefficients remain raw binary32 words. No shader multiplication is applied.
CpuVertexUniformConstruction constructCpuVertexUniformPacket(
        std::uint32_t meshFlags, std::span<const VertexUniformCoefficient> coefficients) {
    CpuVertexUniformConstruction result;
    result.status = "constructed_raw_vertex_uniform_packet";
    result.initialMeshFlags = meshFlags;
    result.constructedMeshFlags = meshFlags & ~std::uint32_t(3);
    result.commandWords = {0x80000007, 0x000F02C0, 0, 0x007F02C1, 0, 0, 0, 0, 0, 0, 0, 0};
    result.coefficients.assign(coefficients.begin(), coefficients.end());
    for (const VertexUniformCoefficient& coefficient : coefficients) {
        require(coefficient.semantic < 12, "Vertex uniform semantic exceeds its instance map");
        require(coefficient.componentCount >= 1 && coefficient.componentCount <= 4,
                "Vertex uniform component count is unsupported");
        if (coefficient.inputRegisterByte & 0x80)
            continue;
        std::size_t slot = result.commandWords.size();
        switch (coefficient.semantic) {
            case 0: slot = 6; break;
            case 1: slot = 5; break;
            case 2: slot = 4; break;
            case 3: slot = 2; break;
            case 4: slot = 10; break;
            case 5: slot = 9; break;
            case 6: slot = 8; break;
            case 8: slot = 7; break;
            default: break;
        }
        if (slot < result.commandWords.size())
            result.commandWords[slot] = coefficient.constant ? 0x3F800000 : coefficient.multiplierWord;
        if (coefficient.componentCount == 4) {
            if (coefficient.semantic == 3) result.constructedMeshFlags |= 1;
            if (coefficient.semantic == 8) result.constructedMeshFlags |= 2;
        }
    }
    return result;
}

// Selection is an explicit result of the optional-archive loader adapter.
// Missing selections never receive constructor or material defaults.
CpuVertexUniformConstruction readCpuVertexUniformConstruction(
        Bytes modelData, const ModelGeometry& model, std::size_t meshIndex,
        Bytes shaderData, const MaterialShaderSelection* selection) {
    CpuVertexUniformConstruction result;
    result.status = "missing_shader_material_selection";
    result.shaderSelectionPresent = selection != nullptr;
    if (!selection)
        return result;
    result.status = selection->status;
    if (!selection->instanceSelected || selection->status != "resource_local_optional_archive_shader_selection")
        return result;
    result.status = "unsupported_model_uniform_layout";
    if (model.status != "resource_local_fields")
        return result;
    require(meshIndex < model.meshes.size(), "Vertex uniform mesh index exceeds its model");
    const ModelMesh& mesh = model.meshes[meshIndex];
    require(mesh.shapeIndex < model.shapes.size() && mesh.materialIndex < model.materials.size(),
            "Vertex uniform mesh binding exceeds its model");
    result.status = "mismatched_shader_material_selection";
    if (selection->modelOffset != model.offset || selection->materialOffset != model.materials[mesh.materialIndex].offset)
        return result;
    result.status = "unsupported_geometry_shader_uniform_state";
    if (selection->geometrySelector >= 0)
        return result;
    const ModelShape& shape = model.shapes[mesh.shapeIndex];
    result.meshOffset = mesh.offset;
    result.shapeOffset = shape.offset;
    result.shaderInstanceOffset = selection->selectedInstanceOffset;
    result.initialMeshFlags = mesh.drawFlagsField;
    ByteReader input(modelData), shader(shaderData);
    require(input.magic(0, "CGFX") && shader.magic(0, "CGFX"), "Vertex uniform inputs require CGFX members");
    input.check(20, input.integer(24));
    shader.check(20, shader.integer(24));
    const std::size_t dataEnd = 20 + input.integer(24);
    const std::size_t shaderEnd = 20 + shader.integer(24);
    auto metadata = [&](std::size_t offset, std::size_t size) {
        require(offset >= 20 && offset <= dataEnd && size <= dataEnd - offset,
                "Vertex uniform metadata escapes DATA");
    };
    auto shaderMetadata = [&](std::size_t offset, std::size_t size) {
        require(offset >= 20 && offset <= shaderEnd && size <= shaderEnd - offset,
                "Vertex uniform shader metadata escapes DATA");
    };
    metadata(mesh.offset, 0x30);
    metadata(shape.offset, 0x40);
    shaderMetadata(selection->selectedInstanceOffset, 0x88);
    require(input.integer(mesh.offset + 0x2C) == mesh.drawFlagsField &&
            input.integer(mesh.offset + 0x18) == mesh.shapeIndex &&
            input.integer(mesh.offset + 0x1C) == mesh.materialIndex && input.relative(mesh.offset + 0x20) == model.offset,
            "Vertex uniform model records disagree with their input");
    require(shader.integer(selection->selectedInstanceOffset) == 3 &&
            shader.relative(selection->selectedInstanceOffset + 0x84) == selection->selectedProgramOffset &&
            std::bit_cast<std::int32_t>(shader.integer(selection->selectedInstanceOffset + 0xC)) == selection->vertexSelector &&
            std::bit_cast<std::int32_t>(shader.integer(selection->selectedInstanceOffset + 0x10)) == selection->geometrySelector,
            "Vertex uniform shader instance disagrees with its selection");
    std::size_t remainingRecords = (dataEnd - 20) / 4;
    auto pointerList = [&](std::size_t field, std::size_t count) {
        metadata(field, 4);
        std::vector<std::size_t> records;
        require(count <= remainingRecords, "Vertex uniform list exceeds its record limit");
        remainingRecords -= count;
        if (!count) {
            require(input.integer(field) == 0, "Empty vertex uniform list has a nonnull pointer");
            return records;
        }
        const std::size_t list = input.relative(field);
        metadata(list, count * 4);
        for (std::size_t index = 0; index < count; ++index) {
            const std::size_t offset = input.relative(list + index * 4);
            metadata(offset, 4);
            records.push_back(offset);
        }
        return records;
    };
    result.status = "unsupported_shape_uniform_layout";
    if (input.integer(shape.offset) != 0x10000001)
        return result;
    const auto groups = pointerList(shape.offset + 0x3C, input.integer(shape.offset + 0x38));
    result.status = "unsupported_empty_vertex_groups";
    if (groups.empty())
        return result;
    std::vector<VertexUniformCoefficient> coefficients;
    for (const std::size_t group : groups) {
        metadata(group, 0x30);
        const std::uint32_t layout = input.integer(group);
        const std::uint32_t flags = input.integer(group + 8);
        result.status = "unsupported_vertex_uniform_group";
        if ((layout != 0x40000002 || flags != 2) && (layout != 0x80000000 || flags != 1))
            return result;
        std::vector<std::size_t> attributes{group};
        if (flags & 2) {
            attributes = pointerList(group + 0x2C, input.integer(group + 0x28));
            if (attributes.empty()) {
                result.status = "unsupported_empty_uniform_attributes";
                return result;
            }
        }
        for (const std::size_t attribute : attributes) {
            metadata(attribute, 0x30);
            require(input.integer(attribute + 8) == ((flags & 1) ? 1u : 0u),
                    "Vertex uniform declaration flags disagree with their group");
            const bool constant = input.integer(attribute + 8) & 1;
            require(constant || input.integer(attribute) == 0x40000001,
                    "Vertex uniform declaration has an unsupported layout");
            const std::uint32_t semantic = input.integer(attribute + 4);
            require(semantic < 12, "Vertex uniform semantic exceeds its instance map");
            const std::uint8_t components = input.byte(attribute + (constant ? 0x10 : 0x28));
            coefficients.push_back({attribute, semantic,
                shader.byte(selection->selectedInstanceOffset + 0x4C + semantic), constant, components,
                constant ? 0x3F800000 : input.integer(attribute + 0x2C)});
        }
    }
    CpuVertexUniformConstruction constructed = constructCpuVertexUniformPacket(mesh.drawFlagsField, coefficients);
    constructed.status = "resource_local_vertex_uniform_construction";
    constructed.shaderSelectionPresent = true;
    constructed.meshOffset = mesh.offset;
    constructed.shapeOffset = shape.offset;
    constructed.shaderInstanceOffset = selection->selectedInstanceOffset;
    return constructed;
}

std::string hexadecimal(const std::string& value) {
    const char* digits = "0123456789abcdef";
    std::string result;
    for (const unsigned char byte : value) {
        result += digits[byte >> 4];
        result += digits[byte & 15];
    }
    return result;
}

std::string integerHexadecimal(std::uint32_t value) {
    std::ostringstream output;
    output << std::hex << std::setfill('0') << std::setw(8) << value;
    return output.str();
}

void writeString(std::ostream& output, const std::string& value) {
    output << '"';
    const char* digits = "0123456789abcdef";
    for (const unsigned char byte : value) {
        if (byte == '"' || byte == '\\') {
            output << '\\' << char(byte);
        } else if (byte < 32 || byte >= 127) {
            output << "\\u00" << digits[byte >> 4] << digits[byte & 15];
        } else {
            output << char(byte);
        }
    }
    output << '"';
}

struct Placement {
    std::string category;
    std::size_t index;
    std::string name;
    std::array<float, 3> position;
    std::array<float, 3> orientation;
    std::array<float, 3> scale;
    Value::Dictionary numericFields;
    Value::Dictionary fields;
};

struct StagePlacement {
    std::map<std::string, std::size_t> categoryCounts;
    std::vector<Placement> placements;
    std::size_t railCount = 0;
    Value::Dictionary rails;
    Value::Array layers;
};

float coordinate(const Value::Dictionary& dictionary, const std::string& key) {
    const auto found = dictionary.find(key);
    require(found != dictionary.end(), "Placement is missing a transform component");
    return found->second.get<float>();
}

StagePlacement readPlacements(const Value::Dictionary& root) {
    StagePlacement result;
    for (const auto& [category, value] : root.at("AllInfos").get<Value::Dictionary>()) {
        const auto& entries = value.get<Value::Array>();
        result.categoryCounts[category] = entries.size();
        for (std::size_t index = 0; index < entries.size(); ++index) {
            const auto& item = entries[index].get<Value::Dictionary>();
            Placement placement{category, index, item.at("name").get<std::string>(), {}, {}, {}, {}, item};
            const std::array<std::string, 3> axes = {"x", "y", "z"};
            for (std::size_t axis = 0; axis < 3; ++axis) {
                placement.position[axis] = coordinate(item, "pos_" + axes[axis]);
                placement.orientation[axis] = coordinate(item, "dir_" + axes[axis]);
                placement.scale[axis] = coordinate(item, "scale_" + axes[axis]);
            }
            for (const auto& [name, field] : item) {
                if (std::holds_alternative<float>(field.data) || std::holds_alternative<std::int32_t>(field.data) ||
                    std::holds_alternative<std::uint32_t>(field.data) || std::holds_alternative<bool>(field.data))
                    placement.numericFields.emplace(name, field);
            }
            result.placements.push_back(std::move(placement));
        }
    }
    if (root.contains("AllRailInfos")) {
        result.rails = root.at("AllRailInfos").get<Value::Dictionary>();
        for (const auto& [name, category] : root.at("AllRailInfos").get<Value::Dictionary>()) {
            static_cast<void>(name);
            result.railCount += category.get<Value::Array>().size();
        }
    }
    if (root.contains("LayerInfos"))
        result.layers = root.at("LayerInfos").get<Value::Array>();
    return result;
}

void writeValue(std::ostream& output, const Value& value) {
    if (std::holds_alternative<std::monostate>(value.data)) {
        output << "null";
    } else if (const bool* boolean = std::get_if<bool>(&value.data)) {
        output << (*boolean ? "true" : "false");
    } else if (const std::int32_t* integer = std::get_if<std::int32_t>(&value.data)) {
        output << "{\"type\":\"int32\",\"value\":" << *integer << '}';
    } else if (const std::uint32_t* integer = std::get_if<std::uint32_t>(&value.data)) {
        output << "{\"type\":\"uint32\",\"value\":" << *integer << '}';
    } else if (const float* floating = std::get_if<float>(&value.data)) {
        output << "{\"type\":\"float32\",\"bits\":";
        writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(*floating)));
        output << '}';
    } else if (const std::string* text = std::get_if<std::string>(&value.data)) {
        output << "{\"type\":\"encoded_string\",\"hex\":";
        writeString(output, hexadecimal(*text));
        output << '}';
    } else if (const Value::Array* array = std::get_if<Value::Array>(&value.data)) {
        output << '[';
        for (std::size_t index = 0; index < array->size(); ++index) {
            if (index)
                output << ',';
            writeValue(output, (*array)[index]);
        }
        output << ']';
    } else {
        output << '{';
        bool first = true;
        for (const auto& [key, field] : value.get<Value::Dictionary>()) {
            if (!first)
                output << ',';
            first = false;
            writeString(output, key);
            output << ':';
            writeValue(output, field);
        }
        output << '}';
    }
}

void writeNumericFields(std::ostream& output, const Value::Dictionary& fields) {
    output << '{';
    bool first = true;
    for (const auto& [name, field] : fields) {
        if (!first)
            output << ',';
        first = false;
        writeString(output, name);
        output << ':';
        if (const float* value = std::get_if<float>(&field.data)) {
            output << "{\"type\":\"float32\",\"bits\":";
            writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(*value)));
            output << '}';
        } else if (const std::int32_t* value = std::get_if<std::int32_t>(&field.data)) {
            output << "{\"type\":\"int32\",\"value\":" << *value << '}';
        } else if (const std::uint32_t* value = std::get_if<std::uint32_t>(&field.data)) {
            output << "{\"type\":\"uint32\",\"value\":" << *value << '}';
        } else {
            output << "{\"type\":\"bool\",\"value\":" << (field.get<bool>() ? "true" : "false") << '}';
        }
    }
    output << '}';
}

template <typename Type>
void writeVector(std::ostream& output, const std::array<Type, 3>& values) {
    output << '[' << values[0] << ',' << values[1] << ',' << values[2] << ']';
}

void writePlacements(std::ostream& output, const StagePlacement& stage) {
    output << "{\"category_counts\":{";
    bool first = true;
    for (const auto& [category, count] : stage.categoryCounts) {
        if (!first)
            output << ',';
        first = false;
        writeString(output, category);
        output << ':' << count;
    }
    output << "},\"rail_count\":" << stage.railCount << ",\"placements\":[";
    first = true;
    for (const Placement& placement : stage.placements) {
        if (!first)
            output << ',';
        first = false;
        output << "{\"category\":";
        writeString(output, placement.category);
        output << ",\"index\":" << placement.index << ",\"name_hex\":";
        writeString(output, hexadecimal(placement.name));
        output << ",\"position\":";
        writeVector(output, placement.position);
        output << ",\"orientation\":";
        writeVector(output, placement.orientation);
        output << ",\"scale\":";
        writeVector(output, placement.scale);
        output << ",\"numeric_fields\":";
        writeNumericFields(output, placement.numericFields);
        output << ",\"fields\":";
        writeValue(output, Value{placement.fields});
        output << '}';
    }
    output << "],\"rails\":";
    writeValue(output, Value{stage.rails});
    output << ",\"layers\":";
    writeValue(output, Value{stage.layers});
    output << '}';
}

void writeCollision(std::ostream& output, const CollisionMesh& mesh) {
    output << "{\"position_count\":" << mesh.positionCount << ",\"normal_count\":" << mesh.normalCount
           << ",\"prism_capacity\":" << mesh.prismCapacity << ",\"referenced_prisms\":" << mesh.triangles.size()
           << ",\"node_count\":" << mesh.nodeCount << ",\"leaf_count\":" << mesh.leafCount
           << ",\"nonzero_leaf_prefixes\":" << mesh.nonzeroLeafPrefixes
           << ",\"maximum_plane_residual\":" << mesh.maximumPlaneResidual << ",\"triangles\":[";
    bool first = true;
    for (const CollisionTriangle& triangle : mesh.triangles) {
        if (!first)
            output << ',';
        first = false;
        output << "{\"prism_index\":" << triangle.prismIndex << ",\"attribute\":" << triangle.attribute
               << ",\"positions\":[";
        for (std::size_t number = 0; number < triangle.positions.size(); ++number) {
            if (number)
                output << ',';
            writeVector(output, triangle.positions[number]);
        }
        output << "]}";
    }
    output << "]}";
}

void writeCpuShaderContextTransfer(std::ostream& output, const CpuShaderContextTransfer& transfer) {
    output << "{\"status\":";
    writeString(output, transfer.status);
    output << ",\"input_ownership\":\"caller_supplied_raw_context\",\"caller_context_present\":"
           << (transfer.callerContextPresent ? "true" : "false") << ",\"material_state_present\":"
           << (transfer.materialStatePresent ? "true" : "false")
           << ",\"active_program\":\"unresolved\",\"shader_arithmetic\":\"unresolved\",\"command_words\":[";
    for (std::size_t index = 0; index < transfer.commandWords.size(); ++index) {
        if (index) output << ',';
        writeString(output, integerHexadecimal(transfer.commandWords[index]));
    }
    output << "],\"geometry_packet_copied\":" << (transfer.commandWords.size() == 12 ? "true" : "false")
           << ",\"cursor_advance_bytes\":" << transfer.cursorAdvanceBytes << ",\"unwritten_trailing_bytes\":0}";
}

void writeModelGeometry(std::ostream& output, const ModelGeometry& model) {
    output << "{\"name_hex\":";
    writeString(output, hexadecimal(model.name));
    output << ",\"offset\":" << model.offset << ",\"flags\":" << model.flags << ",\"status\":";
    writeString(output, model.status);
    if (model.status == "resource_local_fields") {
        output << ",\"model_initialization_fields\":{";
        for (std::size_t field = 0; field < 3; ++field) {
            if (field) output << ',';
            output << (field == 0 ? "\"scale_bits\":[" : field == 1 ? "\"rotation_radian_bits\":["
                                                                              : "\"translation_bits\":[");
            const auto& values = field == 0 ? model.initializationScale
                                           : field == 1 ? model.initializationRotation : model.initializationTranslation;
            for (std::size_t axis = 0; axis < 3; ++axis) {
                if (axis) output << ',';
                writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(values[axis])));
            }
            output << ']';
        }
        output << ",\"matrix_84_bits\":[";
        for (std::size_t index = 0; index < 12; ++index) {
            if (index) output << ',';
            writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(model.initializationMatrix84[index])));
        }
        output << "]}";
    }
    output << ",\"coordinate_space\":\"resource_local\",\"shape_transform\":\"unresolved\","
              "\"bone_transforms\":\"unresolved\",\"material_mapping\":";
    writeString(output, model.materialMappingStatus);
    output << ",\"runtime_mesh_replacement\":\"unresolved\",\"material_state\":\"unresolved\",\"materials\":[";
    bool firstMaterial = true;
    for (const ModelMaterial& material : model.materials) {
        if (!firstMaterial)
            output << ',';
        firstMaterial = false;
        output << "{\"name_hex\":";
        writeString(output, hexadecimal(material.name));
        output << ",\"offset\":" << material.offset << ",\"flags\":" << material.flags
               << ",\"texture_reference_layout\":";
        writeString(output, material.textureReferenceLayout);
        output << ",\"runtime_texture_cache\":\"unresolved\",\"texture_references\":[";
        if (material.textureReferenceLayout == "observed_three_slot_layout")
            for (std::size_t slot = 0; slot < material.textureReferences.size(); ++slot) {
                if (slot)
                    output << ',';
                const MaterialTextureReference& reference = material.textureReferences[slot];
                output << "{\"slot\":" << slot << ",\"status\":";
                writeString(output, reference.status);
                output << ",\"mapper_relative_pointer_field\":" << reference.mapperRelativePointerField;
                if (reference.mapperOffset)
                    output << ",\"mapper_offset\":" << reference.mapperOffset
                           << ",\"mapper_flags\":" << reference.mapperFlags;
                if (reference.referenceOffset) {
                    output << ",\"reference_offset\":" << reference.referenceOffset
                           << ",\"reference_flags\":" << reference.referenceFlags;
                    if (reference.referenceFlags == 0x20000004) {
                        output << ",\"reference_name_present\":" << (reference.referenceNamePresent ? "true" : "false")
                               << ",\"reference_name_hex\":";
                        writeString(output, hexadecimal(reference.referenceName));
                        output << ",\"target_name_hex\":";
                        writeString(output, hexadecimal(reference.targetName));
                        output << ",\"cached_relative_pointer_field\":" << reference.cachedRelativePointerField;
                    }
                }
                if (reference.targetOffset)
                    output << ",\"target_offset\":" << reference.targetOffset
                           << ",\"target_flags\":" << reference.targetFlags;
                output << '}';
            }
        output << "]}";
    }
    output << "],\"meshes\":[";
    bool firstMesh = true;
    for (const ModelMesh& mesh : model.meshes) {
        if (!firstMesh)
            output << ',';
        firstMesh = false;
        output << "{\"name_hex\":";
        writeString(output, hexadecimal(mesh.name));
        output << ",\"offset\":" << mesh.offset << ",\"flags\":" << mesh.flags
               << ",\"draw_flags_field\":" << mesh.drawFlagsField
               << ",\"shape_index\":" << mesh.shapeIndex << ",\"material_index\":" << mesh.materialIndex
               << ",\"parent_offset\":" << mesh.parentOffset
               << ",\"visibility\":\"unresolved\",\"animation\":\"unresolved\",\"shader_boolean_transition\":{"
                  "\"replace_mask\":\"00000186\",\"set_bits\":";
        writeString(output, integerHexadecimal(meshBooleanTransition(mesh, model.shapes[mesh.shapeIndex]).setBits));
        output << ",\"caller_context\":\"unavailable\",\"material_context\":\"unavailable\","
                  "\"active_program\":\"unresolved\",\"shader_arithmetic\":\"unresolved\"}}";
    }
    output << "],\"skeleton\":{\"offset\":" << model.skeleton.offset << ",\"flags\":" << model.skeleton.flags << ",\"status\":";
    writeString(output, model.skeleton.status);
    output << ",\"transform_application\":\"unresolved\"";
    if (model.skeleton.status == "decoded_raw_joint_fields")
        output << ",\"hierarchy_mode_field\":" << model.skeleton.hierarchyMode
               << ",\"transform_flags_field\":" << model.skeleton.transformFlags;
    output << ",\"joints\":[";
    bool firstJoint = true;
    for (const ModelJoint& joint : model.skeleton.joints) {
        if (!firstJoint)
            output << ',';
        firstJoint = false;
        output << "{\"name_hex\":";
        writeString(output, hexadecimal(joint.name));
        output << ",\"offset\":" << joint.offset << ",\"flags\":" << joint.flags << ",\"identifier\":"
               << joint.identifier << ",\"parent_identifier\":" << joint.parentIdentifier
               << ",\"parent_offset\":" << joint.parentOffset << ",\"billboard_field\":" << joint.billboardField;
        for (std::size_t vector = 0; vector < 3; ++vector) {
            output << ",\"" << std::array{"scale_bits", "rotation_bits", "translation_bits"}[vector] << "\":[";
            const auto& fields = vector == 0 ? joint.scale : vector == 1 ? joint.rotation : joint.translation;
            for (std::size_t axis = 0; axis < 3; ++axis) {
                if (axis)
                    output << ',';
                writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(fields[axis])));
            }
            output << ']';
        }
        for (std::size_t matrix = 0; matrix < 3; ++matrix) {
            output << ",\"" << std::array{"matrix_44_bits", "matrix_74_bits", "matrix_a4_bits"}[matrix] << "\":[";
            for (std::size_t component = 0; component < 12; ++component) {
                if (component)
                    output << ',';
                writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(joint.matrixFields[matrix][component])));
            }
            output << ']';
        }
        output << '}';
    }
    output << "]},\"shapes\":[";
    bool firstShape = true;
    for (const ModelShape& shape : model.shapes) {
        if (!firstShape)
            output << ',';
        firstShape = false;
        output << "{\"offset\":" << shape.offset << ",\"flags\":" << shape.flags << ",\"status\":";
        writeString(output, shape.status);
        if (shape.status != "resource_local_fields") {
            output << '}';
            continue;
        }
        output << ",\"position_fields_bits\":[";
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (axis)
                output << ',';
            writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(shape.positionFields[axis])));
        }
        output << ']';
        if (shape.status == "resource_local_fields") {
            // Retail 0x00190F58 transfers these fields to c6 in float32 mode.
            // The shader's arithmetic use of that uniform remains unknown.
            const std::array<std::uint32_t, 6> transferWords{
                (1u << 31) | 6u, (1u << 31) | (4u << 20) | (15u << 16) | 0x2C0u,
                0, std::bit_cast<std::uint32_t>(shape.positionFields[2]),
                std::bit_cast<std::uint32_t>(shape.positionFields[1]),
                std::bit_cast<std::uint32_t>(shape.positionFields[0])};
            output << ",\"vertex_shader_uniform_transfer\":{\"arithmetic_role\":\"unresolved\",\"command_words\":[";
            for (std::size_t index = 0; index < transferWords.size(); ++index) {
                if (index)
                    output << ',';
                writeString(output, integerHexadecimal(transferWords[index]));
            }
            output << "]}";
        }
        output << ",\"vertex_groups\":[";
        bool firstGroup = true;
        for (const ModelVertexGroup& group : shape.vertexGroups) {
            if (!firstGroup)
                output << ',';
            firstGroup = false;
            output << "{\"flags\":" << group.flags << ",\"semantic_field\":" << group.semantic << ",\"status\":";
            writeString(output, group.status);
            if (group.status == "decoded_inline_vector_fields") {
                output << ",\"inline_vector_bits\":[";
                for (std::size_t component = 0; component < 4; ++component) {
                    if (component)
                        output << ',';
                    writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(group.inlineVectorFields[component])));
                }
                output << ']';
            }
            output << ",\"stride\":" << group.stride << ",\"vertex_count\":" << group.vertexCount << ",\"attributes\":[";
            bool firstAttribute = true;
            for (const ModelAttribute& attribute : group.attributes) {
                if (!firstAttribute)
                    output << ',';
                firstAttribute = false;
                output << "{\"semantic\":" << attribute.semantic << ",\"scalar_type\":" << attribute.scalarType
                       << ",\"component_count\":" << attribute.componentCount << ",\"byte_offset\":" << attribute.byteOffset
                       << ",\"multiplier_bits\":";
                writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(attribute.multiplier)));
                output << ",\"raw_components\":[";
                for (std::size_t index = 0; index < attribute.rawComponents.size(); ++index) {
                    if (index)
                        output << ',';
                    output << attribute.rawComponents[index];
                }
                output << "],\"scaled_component_bits\":[";
                for (std::size_t index = 0; index < attribute.scaledComponents.size(); ++index) {
                    if (index)
                        output << ',';
                    writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(attribute.scaledComponents[index])));
                }
                output << "]}";
            }
            output << "]}";
        }
        output << "],\"face_groups\":[";
        bool firstFace = true;
        for (const ModelFaceGroup& group : shape.faceGroups) {
            if (!firstFace)
                output << ',';
            firstFace = false;
            output << "{\"skinning_field\":" << group.skinningField << ",\"bone_references\":[";
            for (std::size_t index = 0; index < group.boneReferences.size(); ++index) {
                if (index)
                    output << ',';
                output << group.boneReferences[index];
            }
            output << "],\"index_streams\":[";
            bool firstStream = true;
            for (const ModelIndexStream& stream : group.streams) {
                if (!firstStream)
                    output << ',';
                firstStream = false;
                output << "{\"scalar_type\":" << stream.scalarType << ",\"primitive_field\":" << stream.primitiveField
                       << ",\"topology\":";
                writeString(output, stream.topology);
                output << ",\"triangle_count\":" << stream.triangleCount
                       << ",\"command_copy_gate_byte\":" << ((stream.primitiveField >> 8) & 0xFF)
                       << ",\"prepared_command_copy_enabled\":"
                       << (((stream.primitiveField >> 8) & 0xFF) != 0 ? "true" : "false")
                       << ",\"caller_geometry_override\":\"unresolved\",\"indices\":[";
                for (std::size_t index = 0; index < stream.indices.size(); ++index) {
                    if (index)
                        output << ',';
                    output << stream.indices[index];
                }
                output << "]}";
            }
            output << "]}";
        }
        output << "]}";
    }
    output << "]}";
}

void writeCatalog(std::ostream& output, const ModelCatalog& catalog) {
    output << "{\"revision\":" << catalog.revision << ",\"categories\":[";
    bool first = true;
    for (const ResourceCategory& category : catalog.categories) {
        if (!first)
            output << ',';
        first = false;
        output << "{\"index\":" << category.index << ",\"entries\":[";
        bool firstEntry = true;
        for (const ResourceEntry& entry : category.entries) {
            if (!firstEntry)
                output << ',';
            firstEntry = false;
            output << "{\"name_hex\":";
            writeString(output, hexadecimal(entry.name));
            output << ",\"offset\":" << entry.offset << '}';
        }
        output << "]}";
    }
    output << "],\"textures\":[";
    for (std::size_t index = 0; index < catalog.textures.size(); ++index) {
        if (index)
            output << ',';
        const TextureImage& texture = catalog.textures[index];
        output << "{\"name_hex\":";
        writeString(output, hexadecimal(texture.name));
        output << ",\"offset\":" << texture.offset << ",\"flags\":" << texture.flags << ",\"status\":";
        writeString(output, texture.status);
        if (texture.imageFieldsDecoded)
            output << ",\"height\":" << texture.height << ",\"width\":" << texture.width
                   << ",\"mipmap_levels\":" << texture.mipmapLevels << ",\"format\":" << texture.format
                   << ",\"descriptor_offset\":" << texture.descriptorOffset
                   << ",\"descriptor_height\":" << texture.descriptorHeight
                   << ",\"descriptor_width\":" << texture.descriptorWidth
                   << ",\"payload_byte_count\":" << texture.payloadByteCount
                   << ",\"payload_offset\":" << texture.payloadOffset
                   << ",\"cached_pointer_field\":" << texture.cachedPointerField;
        output << ",\"storage_decoding\":";
        writeString(output, texture.storageDecodingStatus);
        output << ",\"rgba8_reconstruction\":";
        writeString(output, texture.rgba8ReconstructionStatus);
        if (texture.imageFieldsDecoded && texture.format == 6)
            output << ",\"component_signedness\":\"unresolved\",\"sample_channel_mapping\":\"unresolved\"";
        output << ",\"storage_mipmaps\":[";
        for (std::size_t number = 0; number < texture.storageLevels.size(); ++number) {
            if (number)
                output << ',';
            const TextureStorageLevel& level = texture.storageLevels[number];
            output << "{\"level\":" << level.level << ",\"width\":" << level.width << ",\"height\":" << level.height
                   << ",\"serialized_offset\":" << level.serializedOffset << ",\"byte_count\":" << level.byteCount
                   << ",\"texels\":[";
            for (std::size_t index = 0; index < level.texels.size(); ++index) {
                if (index)
                    output << ',';
                const TextureStorageTexel& texel = level.texels[index];
                output << "{\"storage_x\":" << texel.x << ",\"storage_y\":" << texel.y
                       << ",\"source_offset\":" << texel.sourceOffset << ",\"packed_word\":" << texel.packedWord;
                if (texture.format == 6)
                    output << ",\"component_bytes\":[" << unsigned(texel.componentBytes[0]) << ','
                           << unsigned(texel.componentBytes[1]) << ']';
                else
                    output << ",\"red_integer\":" << unsigned(texel.channels[0])
                           << ",\"green_integer\":" << unsigned(texel.channels[1])
                           << ",\"blue_integer\":" << unsigned(texel.channels[2]);
                output << '}';
            }
            output << "],\"compressed_blocks\":[";
            for (std::size_t index = 0; index < level.blocks.size(); ++index) {
                if (index)
                    output << ',';
                const TextureStorageBlock& block = level.blocks[index];
                output << "{\"storage_x\":" << block.x << ",\"storage_y\":" << block.y
                       << ",\"source_offset\":" << block.sourceOffset << ",\"packed_word_hex\":";
                writeString(output, integerHexadecimal(std::uint32_t(block.packedWord >> 32)) +
                                    integerHexadecimal(std::uint32_t(block.packedWord)));
                output << ",\"differential_mode\":" << (block.differential ? "true" : "false")
                       << ",\"flip_bit\":" << unsigned(block.flipped) << ",\"endpoint_domain\":";
                writeString(output, block.definedEndpoints ? "defined" : "undefined_differential_endpoints");
                output << ",\"table_codewords\":[" << unsigned(block.tableCodewords[0]) << ','
                       << unsigned(block.tableCodewords[1]) << "],\"endpoint_codewords\":[";
                for (std::size_t axis = 0; axis < 3; ++axis) {
                    if (axis)
                        output << ',';
                    output << unsigned(block.endpointCodewords[axis]);
                }
                output << "],\"secondary_codewords\":[";
                for (std::size_t axis = 0; axis < 3; ++axis) {
                    if (axis)
                        output << ',';
                    output << unsigned(block.secondaryCodewords[axis]);
                }
                if (block.differential) {
                    output << "],\"signed_deltas\":[";
                    for (std::size_t axis = 0; axis < 3; ++axis) {
                        if (axis)
                            output << ',';
                        output << int(block.signedDeltas[axis]);
                    }
                }
                output << "],\"selectors\":[";
                for (std::size_t position = 0; position < block.selectors.size(); ++position) {
                    if (position)
                        output << ',';
                    output << unsigned(block.selectors[position]);
                }
                output << ']';
                if (block.hasAlpha) {
                    output << ",\"alpha_source_offset\":" << block.alphaSourceOffset << ",\"alpha_word_hex\":";
                    writeString(output, integerHexadecimal(std::uint32_t(block.alphaWord >> 32)) +
                                        integerHexadecimal(std::uint32_t(block.alphaWord)));
                    output << ",\"alpha_nibbles\":[";
                    for (std::size_t position = 0; position < block.alphaNibbles.size(); ++position) {
                        if (position)
                            output << ',';
                        output << unsigned(block.alphaNibbles[position]);
                    }
                    output << ']';
                }
                output << '}';
            }
            output << "],\"rgba8_pixels_hex\":";
            writeString(output, hexadecimal(std::string(level.rgba8Pixels.begin(), level.rgba8Pixels.end())));
            output << '}';
        }
        const bool reconstructed = !texture.storageLevels.empty() && !texture.storageLevels.front().rgba8Pixels.empty();
        output << "],\"pixel_decoding\":";
        writeString(output, reconstructed ? "public_format_integer_reconstruction" : "unresolved");
        output << ",\"runtime_pointer_application\":\"unresolved\",\"platform_address_translation\":\"unresolved\","
                  "\"color_expansion\":";
        writeString(output, reconstructed ? "public_format_integer_reconstruction" : "unresolved");
        output << ",\"display_orientation\":\"unresolved\",\"gpu_sampling\":\"unresolved\"}";
    }
    output << "],\"models\":[";
    for (std::size_t index = 0; index < catalog.models.size(); ++index) {
        if (index)
            output << ',';
        writeModelGeometry(output, catalog.models[index]);
    }
    output << "]}";
}

class AssetArchive {
    std::vector<std::uint8_t> mData;
    std::map<std::string, Bytes> mMembers;

public:
    explicit AssetArchive(const std::filesystem::path& path) {
        auto source = readFile(path);
        mData = ByteReader(source).magic(0, "Yaz0") ? decompressYaz0(source) : std::move(source);
        for (const ArchiveMember& member : readNarc(mData))
            mMembers.emplace(member.name, member.content);
    }

    Bytes find(const std::string& name) const {
        const auto found = mMembers.find(name);
        return found == mMembers.end() ? Bytes{} : found->second;
    }

    Value table(const std::string& name) const {
        const Bytes content = find(name + ".byml");
        require(!content.empty(), "Required archive table is missing: " + name);
        return ByamlReader(content).read();
    }
};

using Matrix34 = std::array<std::array<float, 4>, 3>;

// VFPv2 rounds the multiply separately from the following add/subtract.
// This native-only setting preserves that rule through optimized Clang builds.
#pragma clang fp contract(off)

float binary32Constant(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}

struct ReducedAngle {
    int quadrant;
    float remainder;
};

// Finite bounded branch shared by retail sine/cosine at 0x00287908/0x00287AD0.
// Larger arguments call __mathlib_rredf2, which remains unreconstructed.
ReducedAngle reduceAngle(float angle) {
    const std::uint32_t bits = std::bit_cast<std::uint32_t>(angle);
    const std::uint32_t magnitude = bits & 0x7FFFFFFF;
    require(magnitude < 0x46490E49, "Angle requires unreconstructed large-argument reduction");
    if (magnitude < 0x3F490FDB)
        return {magnitude < 0x39800000 ? -1 : 0, angle};
    const float scaled = angle * binary32Constant(0x3F22F983);
    const float bias = binary32Constant(0x4B000000);
    float rounded;
    if (bits & 0x80000000) {
        const float biased = scaled - bias;
        rounded = biased + bias;
    } else {
        const float biased = scaled + bias;
        rounded = biased - bias;
    }
    float remainder = angle - rounded * binary32Constant(0x3FC90000);
    remainder -= rounded * binary32Constant(0x39FDA000);
    remainder -= rounded * binary32Constant(0x33A22000);
    remainder -= rounded * binary32Constant(0x2C34611A);
    return {int(rounded) & 3, remainder};
}

float sinePolynomial(float angle) {
    const float square = angle * angle;
    const float first = binary32Constant(0x3C0882DA) - square * binary32Constant(0x394C6D33);
    const float second = binary32Constant(0xBE2AAAA0) + square * first;
    const float scaled = second * square;
    return angle + angle * scaled;
}

float cosinePolynomial(float angle) {
    const float square = angle * angle;
    const float first = binary32Constant(0x3D2A9FCA) + square * binary32Constant(0xBAB23AB9);
    const float second = binary32Constant(0xBEFFFFDD) + square * first;
    return 1.0f + square * second;
}

float sineBinary32(float angle) {
    const auto [quadrant, remainder] = reduceAngle(angle);
    if (quadrant < 0)
        return angle;
    const float value = quadrant & 1 ? cosinePolynomial(remainder) : sinePolynomial(remainder);
    return quadrant & 2 ? -value : value;
}

float cosineBinary32(float angle) {
    const auto [quadrant, remainder] = reduceAngle(angle);
    if (quadrant < 0)
        return 1.0f;
    if (quadrant & 1) {
        const float value = sinePolynomial(remainder);
        return quadrant & 2 ? value : -value;
    }
    const float value = cosinePolynomial(remainder);
    return quadrant & 2 ? -value : value;
}

// TQSV degree/quaternion update: 0x001DB88C -> 0x0026E674.
// Base matrix: 0x00334F90. Scale multiplies its columns at 0x002DDC70.
Matrix34 placementMatrix(const Placement& placement) {
    const float degreeToRadian = binary32Constant(0x3C8EFA35);
    std::array<float, 3> sine, cosine;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        const float radians = placement.orientation[axis] * degreeToRadian;
        const float halfAngle = radians * 0.5f;
        sine[axis] = sineBinary32(halfAngle);
        cosine[axis] = cosineBinary32(halfAngle);
    }
    const float cosineZCosineY = cosine[2] * cosine[1];
    const float sineZCosineY = sine[2] * cosine[1];
    const float cosineZSineY = cosine[2] * sine[1];
    const float sineZSineY = sine[2] * sine[1];
    float x = cosineZCosineY * sine[0];
    float w = cosineZCosineY * cosine[0];
    float z = sineZCosineY * cosine[0];
    float y = cosineZSineY * cosine[0];
    x -= sineZSineY * cosine[0];
    w += sineZSineY * sine[0];
    z -= cosineZSineY * sine[0];
    y += sineZCosineY * sine[0];
    const float xx = x * x * 2.0f;
    const float wx = w * x * 2.0f;
    const float yy = y * y * 2.0f;
    const float wz = w * z * 2.0f;
    const float xy = x * y * 2.0f;
    const float zz = z * z * 2.0f;
    const float xz = x * z * 2.0f;
    const float yz = y * z * 2.0f;
    const float wy = w * y * 2.0f;
    const float oneMinusYY = 1.0f - yy;
    const float oneMinusXX = 1.0f - xx;
    Matrix34 result{{{oneMinusYY - zz, xy - wz, xz + wy, 0},
                     {xy + wz, oneMinusXX - zz, yz - wx, 0},
                     {xz - wy, yz + wx, oneMinusXX - yy, 0}}};
    for (std::size_t row = 0; row < 3; ++row) {
        for (std::size_t column = 0; column < 3; ++column) {
            result[row][column] *= placement.scale[column];
            require(std::isfinite(result[row][column]), "Placement matrix is nonfinite");
        }
        result[row][3] = placement.position[row];
    }
    return result;
}

struct CpuTransformRecord {
    Matrix34 matrix{{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}}};
    std::array<float, 3> scale{1, 1, 1};
    std::uint32_t flags = 0xFE1;
};

using JointRotationTable = std::array<std::array<float, 4>, 256>;

JointRotationTable readJointRotationTable(Bytes executable) {
    require(executable.size() == 3096576 && sha256(executable) ==
            "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64",
            "Joint rotation table requires the verified original EU executable");
    ByteReader input(executable);
    JointRotationTable result{};
    // Original constructor/intake literals reference this owner-local data.
    constexpr std::size_t TableOffset = 0x003A48F4 - 0x00100000;
    for (std::size_t index = 0; index < result.size(); ++index)
        for (std::size_t field = 0; field < 4; ++field)
            result[index][field] = input.floating(TableOffset + index * 16 + field * 4);
    return result;
}

bool boundedJointRotation(float angle) {
    const float scaled = angle * binary32Constant(0x4222F983);
    // Limit this reconstruction to at most fifteen original period subtractions.
    return std::isfinite(scaled) && std::abs(scaled) < 1048576.0f;
}

std::pair<float, float> jointRotationSineCosine(float angle, const JointRotationTable& table) {
    require(boundedJointRotation(angle), "Joint angle exceeds the bounded table-reduction domain");
    const float scaled = angle * binary32Constant(0x4222F983);
    float magnitude = std::abs(scaled);
    while (magnitude >= 65536.0f)
        magnitude -= 65536.0f;
    const std::uint32_t integer = std::uint32_t(magnitude) & 0xFFFF;
    const float fraction = magnitude - float(integer);
    const auto& entry = table[integer & 0xFF];
    float sine = entry[0] + fraction * entry[2];
    const float cosine = entry[1] + fraction * entry[3];
    if (scaled < 0)
        sine = -sine;
    return {sine, cosine};
}

// Shared table arithmetic in model constructor 0x00231660 and joint intake
// 0x002A567C. Products, additions and subtractions round independently.
Matrix34 jointRotationMatrix(const std::array<float, 3>& rotation,
                             const std::array<float, 3>& translation,
                             const JointRotationTable& table) {
    const auto [sineX, cosineX] = jointRotationSineCosine(rotation[0], table);
    const auto [sineY, cosineY] = jointRotationSineCosine(rotation[1], table);
    const auto [sineZ, cosineZ] = jointRotationSineCosine(rotation[2], table);
    const float cosineXSineZ = cosineX * sineZ;
    const float sineXCosineZ = sineX * cosineZ;
    const float cosineXCosineZ = cosineX * cosineZ;
    const float sineXSineZ = sineX * sineZ;
    return {{{cosineZ * cosineY, sineXCosineZ * sineY - cosineXSineZ,
              sineXSineZ + cosineXCosineZ * sineY, translation[0]},
             {sineZ * cosineY, cosineXCosineZ + sineXSineZ * sineY,
              cosineXSineZ * sineY - sineXCosineZ, translation[1]},
             {-sineY, sineX * cosineY, cosineX * cosineY, translation[2]}}};
}

struct CpuModelRoot {
    CpuTransformRecord local;
    CpuTransformRecord composed;
    Matrix34 matrix8c{};
};

CpuModelRoot initializeCpuModelRoot(const ModelGeometry& model, const JointRotationTable& table) {
    CpuModelRoot result;
    result.local = {jointRotationMatrix(model.initializationRotation, model.initializationTranslation, table),
                    model.initializationScale, 0x801};
    result.composed.flags = 0x801;
    for (std::size_t row = 0; row < 3; ++row)
        for (std::size_t column = 0; column < 4; ++column)
            result.matrix8c[row][column] = model.initializationMatrix84[row * 4 + column];
    return result;
}

// LiveActor adapter 0x00129074 falls through into 0x00129080. It copies the
// pose matrix into both records and recomputes only composed scale flags.
void updateCpuModelRoot(CpuModelRoot& root, const Matrix34& poseMatrix,
                        const std::array<float, 3>& scale) {
    root.local.matrix = root.composed.matrix = poseMatrix;
    root.local.scale = root.composed.scale = scale;
    root.local.flags |= 0x800;
    root.composed.flags |= 0x800;
    if (!(root.composed.flags & 8)) {
        root.composed.flags &= ~0x600u;
        if (scale[0] == scale[1] && scale[0] == scale[2]) {
            root.composed.flags |= 0x400;
            if (std::bit_cast<std::uint32_t>(scale[0]) == 0x3F800000)
                root.composed.flags |= 0x200;
        }
    }
    if (!(root.composed.flags & 0x200)) {
        root.matrix8c = poseMatrix;
        for (std::size_t row = 0; row < 3; ++row)
            for (std::size_t column = 0; column < 3; ++column)
                root.matrix8c[row][column] *= scale[column];
    }
}

struct CpuJointPalette {
    std::string status = "not_requested";
    std::vector<CpuTransformRecord> localRecords;
    std::vector<CpuTransformRecord> composedRecords;
    std::vector<Matrix34> primaryMatrices;
    std::vector<Matrix34> secondaryMatrices;
    std::vector<bool> secondaryPresent;
    std::string rootInputOwnership = "controlled_records";
    std::string modelInitializationAdapter = "unreplayed";
    CpuTransformRecord rootLocal{};
    CpuTransformRecord rootComposed{};
    bool rootRecordsPresent = false;
    bool rotationTablePresent = false;
};

struct CpuFaceGroupTransfer {
    std::size_t shapeIndex = 0;
    std::size_t faceGroupIndex = 0;
    std::string status;
    std::vector<std::uint32_t> jointIdentifiers;
    std::vector<std::string> paletteSources;
    std::vector<std::uint32_t> commandWords;
    std::size_t cursorAdvanceBytes = 0;
};

// The original draw loop at 0x00191098 selects buffer identities, then the
// packers at 0x00254D9C/0x00254DD4 transfer reversed binary32 row words.
// These commands establish an interface, without evaluating a vertex shader.
CpuFaceGroupTransfer buildCpuFaceGroupTransfer(const ModelFaceGroup& face,
                                              const ModelSkeleton& skeleton,
                                              const CpuJointPalette& palette,
                                              const Matrix34* modelMatrix8c,
                                              bool skeletalCachePresent = true) {
    CpuFaceGroupTransfer result;
    auto unsupported = [&](const std::string& status) {
        result.status = status;
        result.jointIdentifiers.clear();
        result.paletteSources.clear();
        result.commandWords.clear();
        result.cursorAdvanceBytes = 0;
        return result;
    };
    std::vector<const Matrix34*> matrices;
    if (!skeletalCachePresent || face.boneReferences.empty()) {
        if (!modelMatrix8c)
            return unsupported("unavailable_model_matrix_8c");
        matrices.push_back(modelMatrix8c);
        result.paletteSources.push_back("model_matrix_8c");
    } else {
        // c25..c84 is the resource shader's observed 60-register array.
        // Larger transfers remain raw face fields until their ownership is known.
        if (face.boneReferences.size() > 20)
            return unsupported("unsupported_uniform_palette_extent");
        if (palette.status != "bounded_static_mode_zero_cpu_palette")
            return unsupported("unavailable_cpu_joint_palette");
        for (std::uint32_t identifier : face.boneReferences) {
            if (identifier >= skeleton.joints.size() || skeleton.joints[identifier].identifier != identifier)
                return unsupported("unsupported_joint_buffer_identity");
            const bool secondary = (skeleton.joints[identifier].flags & 0x200) && face.skinningField == 2;
            if (identifier >= palette.primaryMatrices.size() || identifier >= palette.secondaryPresent.size())
                return unsupported("unavailable_cpu_joint_palette");
            if (secondary && (!palette.secondaryPresent[identifier] || identifier >= palette.secondaryMatrices.size()))
                return unsupported("unavailable_secondary_joint_palette");
            matrices.push_back(secondary ? &palette.secondaryMatrices[identifier] : &palette.primaryMatrices[identifier]);
            result.jointIdentifiers.push_back(identifier);
            result.paletteSources.push_back(secondary ? "secondary" : "primary");
        }
    }
    for (const Matrix34* matrix : matrices)
        for (const auto& row : *matrix)
            for (float value : row)
                if (!std::isfinite(value))
                    return unsupported("unsupported_nonfinite_transfer_matrix");
    result.commandWords = {0x80000019, 0x000F02C0};
    for (std::size_t index = 0; index < matrices.size(); ++index)
        for (std::size_t row = 0; row < 3; ++row)
            for (std::size_t column = 4; column-- != 0;) {
                result.commandWords.push_back(std::bit_cast<std::uint32_t>((*matrices[index])[row][column]));
                if (index == 0 && row == 0 && column == 3)
                    result.commandWords.push_back(std::uint32_t((12 * matrices.size() - 1) << 20) | 0x000F02C1);
            }
    // The draw loop advances over a final word without writing it. Do not invent
    // padding bytes or include that word in the recovered command payload.
    result.cursorAdvanceBytes = (result.commandWords.size() + 1) * 4;
    result.status = "bounded_cpu_float32_uniform_transfer";
    return result;
}

std::vector<CpuFaceGroupTransfer> buildCpuModelFaceGroupTransfers(const ModelGeometry& model,
                                                                const CpuJointPalette& palette,
                                                                const Matrix34* modelMatrix8c) {
    std::vector<CpuFaceGroupTransfer> result;
    for (std::size_t shapeIndex = 0; shapeIndex < model.shapes.size(); ++shapeIndex)
        for (std::size_t faceIndex = 0; faceIndex < model.shapes[shapeIndex].faceGroups.size(); ++faceIndex) {
            auto transfer = buildCpuFaceGroupTransfer(model.shapes[shapeIndex].faceGroups[faceIndex],
                                                     model.skeleton, palette, modelMatrix8c);
            transfer.shapeIndex = shapeIndex;
            transfer.faceGroupIndex = faceIndex;
            result.push_back(std::move(transfer));
        }
    return result;
}

void writeCpuFaceGroupTransfers(std::ostream& output, const std::vector<CpuFaceGroupTransfer>& transfers) {
    output << "{\"interface\":\"cpu_float32_uniform_transfer\",\"input_ownership\":\"constructed_cpu_palette_buffers\",\"first_uniform_register\":25,"
              "\"shader_arithmetic\":\"unresolved\",\"active_shader_selection\":\"unresolved\","
              "\"final_vertex_use\":\"unresolved\",\"face_groups\":[";
    for (std::size_t index = 0; index < transfers.size(); ++index) {
        if (index) output << ',';
        const auto& transfer = transfers[index];
        output << "{\"shape_index\":" << transfer.shapeIndex << ",\"face_group_index\":" << transfer.faceGroupIndex
               << ",\"status\":";
        writeString(output, transfer.status);
        output << ",\"joint_identifiers\":[";
        for (std::size_t number = 0; number < transfer.jointIdentifiers.size(); ++number) {
            if (number) output << ',';
            output << transfer.jointIdentifiers[number];
        }
        output << "],\"palette_sources\":[";
        for (std::size_t number = 0; number < transfer.paletteSources.size(); ++number) {
            if (number) output << ',';
            writeString(output, transfer.paletteSources[number]);
        }
        output << "],\"command_words\":[";
        for (std::size_t number = 0; number < transfer.commandWords.size(); ++number) {
            if (number) output << ',';
            writeString(output, integerHexadecimal(transfer.commandWords[number]));
        }
        output << "],\"cursor_advance_bytes\":" << transfer.cursorAdvanceBytes
               << ",\"unwritten_trailing_bytes\":" << (transfer.commandWords.empty() ? 0 : 4) << '}';
    }
    output << "]}";
}

// CPU affine product at 0x00281CF8. Each VMLA rounds its product first.
Matrix34 multiplyAffineBinary32(const Matrix34& left, const Matrix34& right) {
    Matrix34 result{};
    for (std::size_t row = 0; row < 3; ++row) {
        for (std::size_t column = 0; column < 3; ++column) {
            float value = left[row][0] * right[0][column];
            value += left[row][1] * right[1][column];
            value += left[row][2] * right[2][column];
            result[row][column] = value;
        }
        float translation = left[row][3];
        for (std::size_t column = 0; column < 3; ++column)
            translation += left[row][column] * right[column][3];
        result[row][3] = translation;
    }
    return result;
}

// 0x002723E0 scales columns and copies translation unchanged.
Matrix34 scaleMatrixColumns(const Matrix34& matrix, const std::array<float, 3>& scale) {
    Matrix34 result = matrix;
    for (std::size_t row = 0; row < 3; ++row)
        for (std::size_t column = 0; column < 3; ++column)
            result[row][column] = matrix[row][column] * scale[column];
    return result;
}

bool finiteTransformRecord(const CpuTransformRecord& record) {
    for (const auto& row : record.matrix)
        for (float value : row)
            if (!std::isfinite(value))
                return false;
    for (float value : record.scale)
        if (!std::isfinite(value))
            return false;
    return true;
}

// Bounded zero-radian branch of serialized joint intake at 0x002A567C.
// The full intake uses a separate interpolation table, not placement sine/cosine.
CpuTransformRecord zeroRotationJointRecord(const ModelJoint& joint) {
    CpuTransformRecord result;
    result.matrix[2][0] = -0.0f;
    for (std::size_t axis = 0; axis < 3; ++axis)
        result.matrix[axis][3] = joint.translation[axis];
    result.scale = joint.scale;
    result.flags = 0x801;
    for (const auto [inputFlag, outputFlag] : std::array<std::pair<std::uint32_t, std::uint32_t>, 5>{
             {{1, 0x20}, {2, 0x100}, {4, 0x80}, {8, 0x200}, {16, 0x400}}})
        if (joint.flags & inputFlag)
            result.flags |= outputFlag;
    if ((joint.flags & 6) == 6)
        result.flags |= 0x40;
    return result;
}

// Mode-zero helper 0x0025C3E4 preserves its copy/translate/rotate branches.
// A mathematical affine product alone would change its signed-zero schedule.
CpuTransformRecord composeModeZeroJoint(const CpuTransformRecord& local,
                                       const CpuTransformRecord& parentLocal,
                                       const CpuTransformRecord& parentComposed) {
    CpuTransformRecord result;
    // Fresh composed cache records copy the identity initialized by
    // 0x002A2E08..0x002A2E38 with flags 0x7E1, without the dirty bit 0x800.
    result.flags = 0x7E1;
    result.matrix = parentLocal.flags & 0x200 ? parentComposed.matrix
                                            : scaleMatrixColumns(parentComposed.matrix, parentLocal.scale);
    if (!(local.flags & 0x60)) {
        for (std::size_t row = 0; row < 3; ++row) {
            float translation = result.matrix[row][3];
            for (std::size_t column = 0; column < 3; ++column)
                translation += result.matrix[row][column] * local.matrix[column][3];
            result.matrix[row][3] = translation;
        }
        if (!(local.flags & 0x80)) {
            const Matrix34 parent = result.matrix;
            for (std::size_t row = 0; row < 3; ++row)
                for (std::size_t column = 0; column < 3; ++column) {
                    float value = parent[row][0] * local.matrix[0][column];
                    value += parent[row][1] * local.matrix[1][column];
                    value += parent[row][2] * local.matrix[2][column];
                    result.matrix[row][column] = value;
                }
        }
    }
    for (std::size_t axis = 0; axis < 3; ++axis)
        result.scale[axis] = parentComposed.flags & 0x200 ? local.scale[axis]
                                                       : parentComposed.scale[axis] * local.scale[axis];
    // Core 0x003391DC clamps a near-zero aggregate scale using this exact threshold.
    float squaredLength = result.scale[0] * result.scale[0];
    squaredLength += result.scale[1] * result.scale[1];
    squaredLength += result.scale[2] * result.scale[2];
    const float minimum = binary32Constant(0x358637BE);
    if (std::isfinite(squaredLength) && squaredLength < minimum)
        for (float& value : result.scale)
            value = value >= 0 ? minimum : -minimum;
    result.flags &= ~0x7E0u;
    if (result.scale[0] == result.scale[1] && result.scale[0] == result.scale[2]) {
        result.flags |= 0x400;
        if (std::bit_cast<std::uint32_t>(result.scale[0]) == 0x3F800000)
            result.flags |= 0x200;
    }
    return result;
}

CpuJointPalette buildCpuJointPalette(const ModelSkeleton& skeleton, const CpuTransformRecord& rootLocal,
                                    const CpuTransformRecord& rootComposed,
                                    const JointRotationTable* rotationTable = nullptr) {
    CpuJointPalette result;
    result.rootLocal = rootLocal;
    result.rootComposed = rootComposed;
    result.rootRecordsPresent = true;
    result.rotationTablePresent = rotationTable != nullptr;
    if (skeleton.status == "not_present") {
        result.status = "not_present";
        return result;
    }
    auto unsupported = [&](const std::string& status) {
        result.status = status;
        result.localRecords.clear();
        result.composedRecords.clear();
        result.primaryMatrices.clear();
        result.secondaryMatrices.clear();
        result.secondaryPresent.clear();
        return result;
    };
    if (skeleton.status != "decoded_raw_joint_fields")
        return unsupported("unsupported_skeleton_layout");
    // The original dispatch uses the low byte. Higher bits remain raw fields.
    if ((skeleton.hierarchyMode & 0xFF) != 0)
        return unsupported("unsupported_hierarchy_mode");
    if (skeleton.transformFlags & 1)
        return unsupported("unsupported_identity_root_ownership");
    if (!finiteTransformRecord(rootLocal) || !finiteTransformRecord(rootComposed))
        return unsupported("unsupported_nonfinite_root");
    for (std::size_t index = 0; index < skeleton.joints.size(); ++index) {
        const ModelJoint& joint = skeleton.joints[index];
        // The retail loop indexes buffers by parent identifier in dictionary order.
        if (joint.identifier != index || joint.parentIdentifier < -1 ||
            (joint.parentIdentifier >= 0 && std::size_t(joint.parentIdentifier) >= index))
            return unsupported("unsupported_joint_buffer_order");
        if (joint.billboardField & 0xFF)
            return unsupported("unsupported_billboard_update");
        for (float angle : joint.rotation)
            if (!rotationTable && angle != 0)
                return unsupported("unsupported_joint_rotation_table");
        CpuTransformRecord local = zeroRotationJointRecord(joint);
        if (rotationTable) {
            for (float angle : joint.rotation)
                if (!boundedJointRotation(angle))
                    return unsupported("unsupported_joint_rotation_domain");
            local.matrix = jointRotationMatrix(joint.rotation, joint.translation, *rotationTable);
        }
        if (!finiteTransformRecord(local))
            return unsupported("unsupported_nonfinite_joint");
        const CpuTransformRecord& parentLocal = joint.parentIdentifier == -1
                                                   ? rootLocal : result.localRecords[joint.parentIdentifier];
        const CpuTransformRecord& parentComposed = joint.parentIdentifier == -1
                                                      ? rootComposed : result.composedRecords[joint.parentIdentifier];
        const CpuTransformRecord composed = composeModeZeroJoint(local, parentLocal, parentComposed);
        const Matrix34 primary = scaleMatrixColumns(composed.matrix, local.scale);
        CpuTransformRecord primaryCheck{primary, composed.scale, composed.flags};
        if (!finiteTransformRecord(primaryCheck))
            return unsupported("unsupported_nonfinite_composition");
        Matrix34 secondary{};
        const bool secondaryPresent = (joint.flags & 0x240) == 0x240;
        if (secondaryPresent) {
            Matrix34 matrixA4{};
            for (std::size_t row = 0; row < 3; ++row)
                for (std::size_t column = 0; column < 4; ++column)
                    matrixA4[row][column] = joint.matrixFields[2][row * 4 + column];
            secondary = multiplyAffineBinary32(primary, matrixA4);
            if (!finiteTransformRecord({secondary, {1, 1, 1}, 0}))
                return unsupported("unsupported_nonfinite_secondary");
        }
        result.localRecords.push_back(local);
        result.composedRecords.push_back(composed);
        result.primaryMatrices.push_back(primary);
        result.secondaryMatrices.push_back(secondary);
        result.secondaryPresent.push_back(secondaryPresent);
    }
    result.status = "bounded_static_mode_zero_cpu_palette";
    return result;
}

// The original call chain establishes this root update. Actor allocation, complete
// model initialization and game-owned services remain constructed-input limits.
CpuModelRoot placementModelRoot(const Placement& placement, const ModelGeometry& model,
                               const JointRotationTable* rotationTable) {
    Placement unscaled = placement;
    unscaled.scale = {1, 1, 1};
    CpuModelRoot result;
    if (rotationTable)
        result = initializeCpuModelRoot(model, *rotationTable);
    else {
        result.local.flags = result.composed.flags = 0x801;
        for (std::size_t row = 0; row < 3; ++row)
            for (std::size_t column = 0; column < 4; ++column)
                result.matrix8c[row][column] = model.initializationMatrix84[row * 4 + column];
    }
    updateCpuModelRoot(result, placementMatrix(unscaled), placement.scale);
    return result;
}

#pragma clang fp contract(on)

void writeMatrixBits(std::ostream& output, const Matrix34& matrix) {
    output << '[';
    for (std::size_t row = 0; row < 3; ++row)
        for (std::size_t column = 0; column < 4; ++column) {
            if (row || column)
                output << ',';
            writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(matrix[row][column])));
        }
    output << ']';
}

void writeCpuJointPalette(std::ostream& output, const CpuJointPalette& palette) {
    output << "{\"status\":";
    writeString(output, palette.status);
    output << ",\"root_input_ownership\":";
    writeString(output, palette.rootInputOwnership);
    output << ",\"model_initialization_adapter\":";
    writeString(output, palette.modelInitializationAdapter);
    output << ",\"joint_rotation_table\":";
    writeString(output, palette.rotationTablePresent ? "verified_owner_executable" : "not_supplied");
    output << ",\"callbacks\":\"empty\",\"animation\":\"not_applied\","
              "\"billboard_update\":\"not_applied\",\"final_vertex_use\":\"unresolved\"";
    if (palette.rootRecordsPresent) {
        for (std::size_t index = 0; index < 2; ++index) {
            const auto& root = index ? palette.rootComposed : palette.rootLocal;
            output << (index ? ",\"root_composed\":{" : ",\"root_local\":{")
                   << "\"flags\":" << root.flags << ",\"scale_bits\":[";
            for (std::size_t axis = 0; axis < 3; ++axis) {
                if (axis) output << ',';
                writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(root.scale[axis])));
            }
            output << "],\"matrix_bits\":";
            writeMatrixBits(output, root.matrix);
            output << '}';
        }
    }
    output << ",\"joints\":[";
    for (std::size_t index = 0; index < palette.localRecords.size(); ++index) {
        if (index)
            output << ',';
        output << "{\"buffer_index\":" << index;
        for (std::size_t recordIndex = 0; recordIndex < 2; ++recordIndex) {
            const auto& record = recordIndex ? palette.composedRecords[index] : palette.localRecords[index];
            output << (recordIndex ? ",\"composed\":{" : ",\"local\":{")
                   << "\"flags\":" << record.flags << ",\"scale_bits\":[";
            for (std::size_t axis = 0; axis < 3; ++axis) {
                if (axis)
                    output << ',';
                writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(record.scale[axis])));
            }
            output << "],\"matrix_bits\":";
            writeMatrixBits(output, record.matrix);
            output << '}';
        }
        output << ",\"primary_matrix_bits\":";
        writeMatrixBits(output, palette.primaryMatrices[index]);
        output << ",\"secondary_matrix_status\":";
        writeString(output, palette.secondaryPresent[index] ? "primary_times_serialized_a4" : "not_requested");
        if (palette.secondaryPresent[index]) {
            output << ",\"secondary_matrix_bits\":";
            writeMatrixBits(output, palette.secondaryMatrices[index]);
        }
        output << '}';
    }
    output << "]}";
}

Vector3 transformPoint(const Matrix34& matrix, const Vector3& position) {
    Vector3 result{};
    for (std::size_t row = 0; row < 3; ++row) {
        result[row] = matrix[row][3];
        for (std::size_t column = 0; column < 3; ++column)
            result[row] += double(matrix[row][column]) * position[column];
        require(std::isfinite(result[row]), "Transformed collision point is nonfinite");
    }
    return result;
}

struct SceneInstance {
    std::size_t placementIndex;
    std::string actorClass;
    std::string status;
    std::string archive;
    std::string model;
    std::string collision;
    std::string collisionStatus;
    Matrix34 matrix{};
    CollisionMesh collisionMesh{};
    CpuJointPalette cpuJointPalette{};
    std::vector<CpuFaceGroupTransfer> cpuFaceGroupTransfers;
};

struct SceneDefinition {
    StagePlacement stage;
    std::vector<SceneInstance> instances;
};

std::string numberedName(const std::string& name, std::int32_t number) {
    std::ostringstream output;
    output << name << std::internal << std::setfill('0') << std::setw(3) << number;
    return output.str();
}

SceneDefinition readScene(const std::filesystem::path& factoryPath,
                          const std::filesystem::path& stagePath,
                          const std::vector<std::filesystem::path>& resourcePaths,
                          const JointRotationTable* rotationTable = nullptr) {
    AssetArchive factoryArchive(factoryPath), stageArchive(stagePath);
    const Value factoryRoot = factoryArchive.table("CreatorClassNameTable");
    std::map<std::string, std::string> actorClasses;
    for (const Value& record : factoryRoot.get<Value::Array>()) {
        const auto& item = record.get<Value::Dictionary>();
        const std::string& name = item.at("ObjectName").get<std::string>();
        const std::string& actorClass = item.at("ClassName").get<std::string>();
        require(!name.empty() && !actorClass.empty(), "Empty actor factory name");
        // getCreator at 0x00268EB0 returns the first matching conversion entry.
        actorClasses.emplace(name, actorClass);
    }
    std::map<std::string, AssetArchive> resources;
    for (const auto& path : resourcePaths) {
        const std::string directory = path.parent_path().filename().string();
        require(directory == "ObjectData" || directory == "MapPartsData",
                "Scene resource must be in ObjectData or MapPartsData");
        const std::string key = directory + "/" + path.stem().string();
        require(resources.try_emplace(key, path).second, "Duplicate scene archive key: " + key);
    }
    const Value stageRoot = stageArchive.table("StageData");
    SceneDefinition result{readPlacements(stageRoot.get<Value::Dictionary>()), {}};
    for (std::size_t index = 0; index < result.stage.placements.size(); ++index) {
        const Placement& placement = result.stage.placements[index];
        SceneInstance instance{index, {}, "unresolved_placement_category", {}, {}, {}, {}, {}, {}, {}, {}};
        if (placement.category != "ObjInfo") {
            result.instances.push_back(std::move(instance));
            continue;
        }
        instance.status = "unresolved_actor_factory_entry";
        const auto creator = actorClasses.find(placement.name);
        if (creator == actorClasses.end()) {
            result.instances.push_back(std::move(instance));
            continue;
        }
        instance.actorClass = creator->second;
        instance.status = "unresolved_actor_initializer";
        if (instance.actorClass != "FixMapParts") {
            result.instances.push_back(std::move(instance));
            continue;
        }
        // FixMapParts creator 0x00396404, ctor 0x0012C328, init 0x0012C228.
        // The init calls the map-part helper at 0x002D5664/0x002D5668.
        const auto shape = placement.fields.find("ShapeModelNo");
        const std::int32_t shapeNumber = shape == placement.fields.end() ? -1 : shape->second.get<std::int32_t>();
        const std::string modelName = shapeNumber == -1 ? placement.name : numberedName(placement.name, shapeNumber);
        instance.archive = shapeNumber >= 0 ? "MapPartsData/" + modelName : "ObjectData/" + placement.name;
        instance.matrix = placementMatrix(placement);
        instance.status = "unresolved_archive";
        const auto resource = resources.find(instance.archive);
        if (resource == resources.end()) {
            result.instances.push_back(std::move(instance));
            continue;
        }
        const Value initialization = resource->second.table("InitActor");
        const auto& parameters = initialization.get<Value::Dictionary>();
        // InitActor dispatch at 0x002417E8 tests key presence, including null Model.
        instance.status = "resolved_fixed_resource_binding";
        const auto pose = parameters.find("Pose");
        if (pose != parameters.end() && std::holds_alternative<std::string>(pose->second.data))
            instance.status = "unresolved_pose_override";
        if (parameters.contains("Model")) {
            instance.model = modelName + ".bcmdl";
            const Bytes model = resource->second.find(instance.model);
            if (model.empty()) {
                instance.status = "unresolved_model_resource";
            } else {
                const ModelCatalog catalog = readCgfx(model);
                bool foundModel = false;
                for (const ResourceCategory& category : catalog.categories)
                    if (category.index == 0)
                        for (const ResourceEntry& entry : category.entries)
                            foundModel |= entry.name == modelName;
                if (!foundModel)
                    instance.status = "unresolved_model_identity";
                if (foundModel && instance.status == "resolved_fixed_resource_binding") {
                    const auto selected = std::find_if(catalog.models.begin(), catalog.models.end(),
                        [&](const ModelGeometry& geometry) { return geometry.name == modelName; });
                    if (selected != catalog.models.end()) {
                        bool supportedModelRotation = true;
                        if (rotationTable)
                            for (float angle : selected->initializationRotation)
                                supportedModelRotation &= boundedJointRotation(angle);
                        if (supportedModelRotation) {
                            const CpuModelRoot root = placementModelRoot(placement, *selected, rotationTable);
                            instance.cpuJointPalette = buildCpuJointPalette(selected->skeleton, root.local,
                                                                           root.composed, rotationTable);
                            instance.cpuFaceGroupTransfers = buildCpuModelFaceGroupTransfers(*selected,
                                                                                           instance.cpuJointPalette,
                                                                                           &root.matrix8c);
                        } else {
                            instance.cpuJointPalette.status = "unsupported_model_rotation_domain";
                            instance.cpuJointPalette.rotationTablePresent = true;
                            instance.cpuFaceGroupTransfers = buildCpuModelFaceGroupTransfers(*selected,
                                                                                           instance.cpuJointPalette,
                                                                                           nullptr);
                        }
                        instance.cpuJointPalette.rootInputOwnership = "constructed_actor_pose_model_inputs";
                        instance.cpuJointPalette.modelInitializationAdapter = "bounded_original_animation_call_chain";
                    }
                }
            }
        }
        instance.collisionStatus = "not_requested";
        if (parameters.contains("Collision")) {
            std::string collisionName = instance.archive.substr(instance.archive.find_last_of('/') + 1);
            const auto* options = std::get_if<Value::Dictionary>(&parameters.at("Collision").data);
            bool followsJoint = false;
            if (options) {
                const auto name = options->find("Name");
                if (name != options->end())
                    if (const auto* text = std::get_if<std::string>(&name->second.data))
                        collisionName = *text;
                const auto joint = options->find("Joint");
                followsJoint = joint != options->end() && std::holds_alternative<std::string>(joint->second.data);
            }
            // 0x00242014 defaults Name to resource basename; 0x0024F8CC uses .kcl.
            instance.collision = collisionName + ".kcl";
            const Bytes collision = resource->second.find(instance.collision);
            instance.collisionStatus = collision.empty() ? "absent_member" : "initial_pose_geometry";
            if (followsJoint)
                instance.collisionStatus = "unresolved_joint_transform";
            else if (!collision.empty() && instance.status != "resolved_fixed_resource_binding")
                instance.collisionStatus = "unresolved_initial_transform";
            if (!collision.empty() && !followsJoint && instance.status == "resolved_fixed_resource_binding") {
                instance.collisionMesh = readKcl(collision);
                for (CollisionTriangle& triangle : instance.collisionMesh.triangles)
                    for (Vector3& position : triangle.positions)
                        position = transformPoint(instance.matrix, position);
            }
        }
        result.instances.push_back(std::move(instance));
    }
    return result;
}

void writeScene(std::ostream& output, const SceneDefinition& scene) {
    output << "{\"placement_source\":\"AllInfos\",\"layer_filter_applied\":false,"
              "\"collision_activation\":\"unresolved_stage_switch_state\","
              "\"trigonometry\":\"reconstructed_bounded_binary32\","
              "\"rounding_mode\":\"requires_round_to_nearest_even\",\"stage\":";
    writePlacements(output, scene.stage);
    output << ",\"instances\":[";
    for (std::size_t number = 0; number < scene.instances.size(); ++number) {
        if (number)
            output << ',';
        const SceneInstance& instance = scene.instances[number];
        output << "{\"placement_index\":" << instance.placementIndex << ",\"actor_class_hex\":";
        writeString(output, hexadecimal(instance.actorClass));
        output << ",\"status\":";
        writeString(output, instance.status);
        output << ",\"archive_hex\":";
        writeString(output, hexadecimal(instance.archive));
        output << ",\"model_member_hex\":";
        writeString(output, hexadecimal(instance.model));
        output << ",\"collision_member_hex\":";
        writeString(output, hexadecimal(instance.collision));
        output << ",\"collision_status\":";
        writeString(output, instance.collisionStatus);
        output << ",\"cpu_joint_palette\":";
        writeCpuJointPalette(output, instance.cpuJointPalette);
        output << ",\"cpu_face_group_transfers\":";
        writeCpuFaceGroupTransfers(output, instance.cpuFaceGroupTransfers);
        if (!instance.archive.empty()) {
            output << ",\"matrix\":[";
            for (std::size_t row = 0; row < 3; ++row) {
                if (row)
                    output << ',';
                output << '[';
                for (std::size_t column = 0; column < 4; ++column) {
                    if (column)
                        output << ',';
                    output << instance.matrix[row][column];
                }
                output << ']';
            }
            output << ']';
            output << ",\"matrix_bits\":[";
            for (std::size_t row = 0; row < 3; ++row) {
                if (row)
                    output << ',';
                output << '[';
                for (std::size_t column = 0; column < 4; ++column) {
                    if (column)
                        output << ',';
                    writeString(output, integerHexadecimal(std::bit_cast<std::uint32_t>(instance.matrix[row][column])));
                }
                output << ']';
            }
            output << ']';
        }
        if (!instance.collisionMesh.triangles.empty()) {
            output << ",\"plane_residual_space\":\"resource_local\"";
            output << ",\"initial_collision_geometry\":";
            writeCollision(output, instance.collisionMesh);
        }
        output << '}';
    }
    output << "]}";
}

void writeMaterialShaderSelections(std::ostream& output, const std::vector<MaterialShaderSelection>& selections) {
    output << '[';
    for (std::size_t index = 0; index < selections.size(); ++index) {
        if (index) output << ',';
        const MaterialShaderSelection& selection = selections[index];
        output << "{\"model_offset\":" << selection.modelOffset << ",\"material_offset\":" << selection.materialOffset
               << ",\"optional_shader_archive_present\":" << (selection.optionalShaderArchivePresent ? "true" : "false")
               << ",\"status\":";
        writeString(output, selection.status);
        output << ",\"reference_relative_pointer_field\":" << selection.referenceRelativePointerField
               << ",\"reference_offset\":" << selection.referenceOffset << ",\"reference_flags\":" << selection.referenceFlags
               << ",\"reference_name_present\":" << (selection.referenceNamePresent ? "true" : "false")
               << ",\"reference_name_hex\":";
        writeString(output, hexadecimal(selection.referenceName));
        output << ",\"cached_relative_pointer_field\":" << selection.cachedRelativePointerField
               << ",\"instance_index\":" << selection.instanceIndex
               << ",\"instance_selected\":" << (selection.instanceSelected ? "true" : "false")
               << ",\"runtime_shader_cache\":\"unapplied\",\"full_scene_initialization\":\"unreplayed\","
                  "\"active_program\":\"unresolved\",\"shader_arithmetic\":\"unresolved\"";
        if (!selection.selectedProgramName.empty()) {
            output << ",\"selected_program_resource\":\"optional_shader_archive\",\"selected_program_name_hex\":";
            writeString(output, hexadecimal(selection.selectedProgramName));
            output << ",\"selected_program_offset\":" << selection.selectedProgramOffset;
        }
        if (selection.instanceSelected)
            output << ",\"selected_instance_offset\":" << selection.selectedInstanceOffset
                   << ",\"vertex_selector\":" << selection.vertexSelector << ",\"geometry_selector\":" << selection.geometrySelector
                   << ",\"vertex_executable_offset\":" << selection.vertexExecutableOffset
                   << ",\"geometry_executable_offset\":" << selection.geometryExecutableOffset
                   << ",\"same_as_named_initializer_instance\":" << (selection.sameAsNamedInitializerInstance ? "true" : "false");
        output << '}';
    }
    output << ']';
}

void inspectFile(std::ostream& output, const std::filesystem::path& path) {
    auto compressed = readFile(path);
    ByteReader raw(compressed);
    auto decoded = raw.magic(0, "Yaz0") ? decompressYaz0(compressed) : std::move(compressed);
    ByteReader input(decoded);
    output << "{\"path\":";
    writeString(output, path.generic_string());
    output << ",\"decoded_size\":" << decoded.size();
    if (!input.magic(0, "NARC")) {
        if (input.magic(0, "CGFX")) {
            output << ",\"catalog\":";
            writeCatalog(output, readCgfx(decoded));
        } else if (input.magic(0, "YB") || input.magic(0, "BY")) {
            const Value root = ByamlReader(decoded).read();
            const auto* dictionary = std::get_if<Value::Dictionary>(&root.data);
            require(dictionary && dictionary->contains("AllInfos"), "Raw BYAML is not a placement table");
            output << ",\"stage\":";
            writePlacements(output, readPlacements(*dictionary));
        } else {
            // Raw KCL is recognized by its independently verified 0x38 header.
            require(input.integer(0) == 0x38, "Unrecognized asset format");
            output << ",\"collision\":";
            writeCollision(output, readKcl(decoded));
        }
        output << '}';
        return;
    }
    std::vector<ArchiveMember> members;
    try {
        members = readNarc(decoded);
    } catch (const UnsupportedArchive& error) {
        output << ",\"status\":\"unsupported_nested_archive\",\"reason\":";
        writeString(output, error.what());
        output << '}';
        return;
    }
    output << ",\"status\":\"decoded\",\"members\":[";
    bool first = true;
    for (const ArchiveMember& member : members) {
        if (!first)
            output << ',';
        first = false;
        output << "{\"name\":";
        writeString(output, member.name);
        output << ",\"size\":" << member.content.size();
        ByteReader content(member.content);
        if (member.name.ends_with(".byml")) {
            const Value root = ByamlReader(member.content).read();
            output << ",\"byaml_valid\":true";
            const auto* dictionary = std::get_if<Value::Dictionary>(&root.data);
            if (dictionary && dictionary->contains("AllInfos")) {
                output << ",\"stage\":";
                writePlacements(output, readPlacements(*dictionary));
            }
        } else if (member.name.ends_with(".kcl")) {
            output << ",\"collision\":";
            writeCollision(output, readKcl(member.content));
        } else if (content.magic(0, "CGFX")) {
            output << ",\"catalog\":";
            writeCatalog(output, readCgfx(member.content));
        }
        output << '}';
    }
    output << "]}";
}

} // namespace runtime

int main(int argumentCount, char** arguments) {
    if (argumentCount < 2) {
        std::cerr << "Usage: asset_reader <verified local archive or asset>...\n"
                  << "       asset_reader --scene <factory archive> <stage archive> <resource archives>...\n"
                  << "       asset_reader --scene-with-executable <EU code.bin> <factory archive> <stage archive> <resource archives>...\n"
                  << "Write the JSON report only to ignored data/build storage.\n";
        return 2;
    }
    try {
        std::ostringstream report;
        const std::string mode = arguments[1];
        if (mode == "--scene" || mode == "--scene-with-executable") {
            const bool withExecutable = mode == "--scene-with-executable";
            const int firstArchive = withExecutable ? 3 : 2;
            runtime::require(argumentCount >= firstArchive + 2, "Scene mode requires factory and stage archives");
            runtime::JointRotationTable table{};
            if (withExecutable)
                table = runtime::readJointRotationTable(runtime::readFile(arguments[2]));
            std::vector<std::filesystem::path> resources;
            for (int number = firstArchive + 2; number < argumentCount; ++number)
                resources.emplace_back(arguments[number]);
            report << std::setprecision(std::numeric_limits<double>::max_digits10)
                   << "{\"schema_version\":1,\"string_storage\":\"raw_encoded_bytes\",\"scene\":";
            runtime::writeScene(report, runtime::readScene(arguments[firstArchive], arguments[firstArchive + 1],
                                                         resources, withExecutable ? &table : nullptr));
            report << "}\n";
            std::cout << report.str();
            return 0;
        }
        report << std::setprecision(std::numeric_limits<double>::max_digits10)
               << "{\"schema_version\":1,\"string_storage\":\"raw_encoded_bytes\",\"files\":[";
        for (int number = 1; number < argumentCount; ++number) {
            if (number != 1)
                report << ',';
            try {
                runtime::inspectFile(report, arguments[number]);
            } catch (const std::exception& error) {
                throw runtime::FormatError(std::string(arguments[number]) + ": " + error.what());
            }
        }
        report << "]}\n";
        std::cout << report.str();
    } catch (const std::exception& error) {
        std::cerr << "asset_reader: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
