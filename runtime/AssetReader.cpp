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
};

struct ModelSkeleton {
    std::size_t offset;
    std::uint32_t flags;
    std::string status;
    std::vector<ModelJoint> joints;
};

struct ModelMaterial {
    std::string name;
    std::size_t offset;
    std::uint32_t flags;
};

struct ModelMesh {
    std::string name;
    std::size_t offset;
    std::uint32_t flags;
    std::uint32_t shapeIndex;
    std::uint32_t materialIndex;
    std::size_t parentOffset;
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
};

// The owner's revision uses OpenGL scalar enums in the documented CGFX records.
// These records retain resource-local data. Bone, shape and draw semantics stay open.
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
    if (result.flags & 0x80) {
        metadata(entry.offset, 0xE4);
        const std::size_t skeleton = input.relative(entry.offset + 0xE0);
        metadata(skeleton, 8);
        require(input.magic(skeleton + 4, "SOBJ"), "CGFX skeleton has no SOBJ signature");
        result.skeleton = {skeleton, input.integer(skeleton), "unresolved_skeleton_layout", {}};
        if (result.skeleton.flags == 0x02000000) {
            metadata(skeleton, 0x20);
            result.skeleton.status = "decoded_raw_joint_fields";
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
                                     std::bit_cast<std::int32_t>(input.integer(bone + 0xC)), 0, {}, {}, {}, {}};
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
            result.materials.push_back({name, material, input.integer(material)});
        }
    } else {
        require(input.integer(entry.offset + 0xC0) == 0, "Empty CGFX material dictionary has a nonnull pointer");
    }
    std::set<std::size_t> meshOffsets;
    for (const std::size_t mesh : pointerList(entry.offset + 0xB8, input.integer(entry.offset + 0xB4))) {
        metadata(mesh, 0x24);
        require(input.magic(mesh + 4, "SOBJ"), "CGFX mesh has no SOBJ signature");
        require(input.integer(mesh) == 0x01000000, "Unsupported CGFX mesh identity layout");
        require(meshOffsets.insert(mesh).second, "Duplicate CGFX mesh record");
        const std::size_t nameOffset = input.relative(mesh + 0xC);
        metadata(nameOffset, 1);
        ModelMesh record{input.terminatedText(nameOffset, dataEnd - nameOffset), mesh, input.integer(mesh),
                         input.integer(mesh + 0x18), input.integer(mesh + 0x1C), input.relative(mesh + 0x20)};
        // Retail 0x0033549C uses these two indices in the model's own lists.
        require(record.shapeIndex < result.shapes.size(), "CGFX mesh shape index exceeds its model");
        require(record.materialIndex < result.materials.size(), "CGFX mesh material index exceeds its model");
        require(record.parentOffset == entry.offset, "CGFX mesh parent disagrees with its model");
        result.meshes.push_back(std::move(record));
    }
    result.materialMappingStatus = "model_local_indices";
    return result;
}

struct ModelCatalog {
    std::uint32_t revision;
    std::vector<ResourceCategory> categories;
    std::vector<ModelGeometry> models;
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
    ModelCatalog result{input.integer(8), {}, {}};
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
            if (number == 0)
                result.models.push_back(readModelGeometry(data, 20 + dataSize, imageStart,
                                                          result.revision, category.entries.back()));
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

void writeModelGeometry(std::ostream& output, const ModelGeometry& model) {
    output << "{\"name_hex\":";
    writeString(output, hexadecimal(model.name));
    output << ",\"offset\":" << model.offset << ",\"flags\":" << model.flags << ",\"status\":";
    writeString(output, model.status);
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
        output << ",\"offset\":" << material.offset << ",\"flags\":" << material.flags << '}';
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
               << ",\"shape_index\":" << mesh.shapeIndex << ",\"material_index\":" << mesh.materialIndex
               << ",\"parent_offset\":" << mesh.parentOffset
               << ",\"visibility\":\"unresolved\",\"animation\":\"unresolved\"}";
    }
    output << "],\"skeleton\":{\"offset\":" << model.skeleton.offset << ",\"flags\":" << model.skeleton.flags << ",\"status\":";
    writeString(output, model.skeleton.status);
    output << ",\"transform_application\":\"unresolved\",\"joints\":[";
    bool firstJoint = true;
    for (const ModelJoint& joint : model.skeleton.joints) {
        if (!firstJoint)
            output << ',';
        firstJoint = false;
        output << "{\"name_hex\":";
        writeString(output, hexadecimal(joint.name));
        output << ",\"offset\":" << joint.offset << ",\"flags\":" << joint.flags << ",\"identifier\":"
               << joint.identifier << ",\"parent_identifier\":" << joint.parentIdentifier
               << ",\"parent_offset\":" << joint.parentOffset;
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
        output << "],\"vertex_groups\":[";
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

#pragma clang fp contract(on)

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
                          const std::vector<std::filesystem::path>& resourcePaths) {
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
        SceneInstance instance{index, {}, "unresolved_placement_category", {}, {}, {}, {}, {}, {}};
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
                  << "Write the JSON report only to ignored data/build storage.\n";
        return 2;
    }
    try {
        std::ostringstream report;
        if (std::string(arguments[1]) == "--scene") {
            runtime::require(argumentCount >= 4, "Scene mode requires factory and stage archives");
            std::vector<std::filesystem::path> resources;
            for (int number = 4; number < argumentCount; ++number)
                resources.emplace_back(arguments[number]);
            report << std::setprecision(std::numeric_limits<double>::max_digits10)
                   << "{\"schema_version\":1,\"string_storage\":\"raw_encoded_bytes\",\"scene\":";
            runtime::writeScene(report, runtime::readScene(arguments[2], arguments[3], resources));
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
