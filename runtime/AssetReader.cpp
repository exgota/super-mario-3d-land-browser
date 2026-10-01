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

struct ModelCatalog {
    std::uint32_t revision;
    std::vector<ResourceCategory> categories;
};

ModelCatalog readCgfx(Bytes data) {
    ByteReader input(data);
    require(input.magic(0, "CGFX") && input.byte(4) == 0xFF && input.byte(5) == 0xFE &&
            input.integer(6, 2) == 20 && input.integer(12) == data.size(),
            "Unsupported or invalid CGFX header");
    const std::size_t sectionCount = input.integer(16);
    require(sectionCount >= 1 && sectionCount <= 2, "Unsupported CGFX section count");
    std::size_t section = 20;
    std::size_t dataSize = 0;
    for (std::size_t number = 0; number < sectionCount; ++number) {
        require(input.magic(section, number == 0 ? "DATA" : "IMAG"), "Unexpected CGFX section");
        const std::size_t size = input.integer(section + 4);
        require(size >= 8, "Invalid CGFX section size");
        input.check(section, size);
        if (number == 0)
            dataSize = size;
        section += size;
    }
    require(section == data.size(), "CGFX sections do not cover their file");
    require(dataSize >= 8 + 16 * 8, "CGFX DATA catalog is truncated");
    ModelCatalog result{input.integer(8), {}};
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
    return result;
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
};

struct StagePlacement {
    std::map<std::string, std::size_t> categoryCounts;
    std::vector<Placement> placements;
    std::size_t railCount = 0;
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
            Placement placement{category, index, item.at("name").get<std::string>(), {}, {}, {}, {}};
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
        for (const auto& [name, category] : root.at("AllRailInfos").get<Value::Dictionary>()) {
            static_cast<void>(name);
            result.railCount += category.get<Value::Array>().size();
        }
    }
    return result;
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
        output << '}';
    }
    output << "]}";
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
    output << "]}";
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
                  << "Write the JSON report only to ignored data/build storage.\n";
        return 2;
    }
    try {
        std::ostringstream report;
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
