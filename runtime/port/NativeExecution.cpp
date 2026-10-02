// Execute ahead-of-time translated ARM code. Unsupported CPU and platform work stops.
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include "recomp.h"

namespace {
struct Region {
    std::uint32_t base;
    std::vector<std::uint8_t> bytes;
    bool writable;
};
struct Machine {
    std::vector<Region> regions;
    const Entry* entries;
    std::uint32_t count;
};

[[noreturn]] void fail(const std::string& message) {
    std::cerr << message << '\n';
    std::exit(3);
}

std::vector<std::uint8_t> readFile(const char* path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) fail(std::string("cannot read ") + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::uint32_t word(const std::vector<std::uint8_t>& bytes, std::size_t offset) {
    if (offset + 4 > bytes.size()) fail("truncated header");
    return bytes[offset] | std::uint32_t(bytes[offset + 1]) << 8 |
           std::uint32_t(bytes[offset + 2]) << 16 | std::uint32_t(bytes[offset + 3]) << 24;
}

std::uint8_t* access(Context* context, std::uint32_t address, std::size_t size, bool write) {
    auto& machine = *static_cast<Machine*>(context->user);
    for (auto& region : machine.regions) {
        const auto offset = std::uint64_t(address) - region.base;
        if (address >= region.base && offset + size <= region.bytes.size()) {
            if (write && !region.writable) fail("write to read-only guest region at " + std::to_string(address));
            return region.bytes.data() + offset;
        }
    }
    fail("unmapped guest memory at " + std::to_string(address));
}
std::uint8_t read8(Context* c, std::uint32_t a) { return *access(c, a, 1, false); }
std::uint16_t read16(Context* c, std::uint32_t a) {
    auto* b = access(c, a, 2, false);
    return b[0] | std::uint16_t(b[1]) << 8;
}
std::uint32_t read32(Context* c, std::uint32_t a) {
    auto* b = access(c, a, 4, false);
    return b[0] | std::uint32_t(b[1]) << 8 | std::uint32_t(b[2]) << 16 | std::uint32_t(b[3]) << 24;
}
void write8(Context* c, std::uint32_t a, std::uint8_t v) { *access(c, a, 1, true) = v; }
void write16(Context* c, std::uint32_t a, std::uint16_t v) {
    auto* b = access(c, a, 2, true);
    for (int i = 0; i < 2; ++i) b[i] = static_cast<std::uint8_t>(v >> (i * 8));
}
void write32(Context* c, std::uint32_t a, std::uint32_t v) {
    auto* b = access(c, a, 4, true);
    for (int i = 0; i < 4; ++i) b[i] = static_cast<std::uint8_t>(v >> (i * 8));
}
void interpret(Context*, std::uint32_t address, std::uint32_t) {
    fail("unsupported translated instruction at " + std::to_string(address) + "; interpreter fallback refused");
}
Code lookup(Context* context, std::uint32_t address) {
    auto& machine = *static_cast<Machine*>(context->user);
    const auto* end = machine.entries + machine.count;
    const auto* found = std::lower_bound(machine.entries, end, address,
        [](const Entry& entry, std::uint32_t key) { return entry.address < key; });
    return found != end && found->address == address ? found->code : nullptr;
}
const Host host{read8, read16, read32, write8, write16, write32, interpret, lookup};

template<class T> T symbol(void* library, const char* name) {
    auto* pointer = dlsym(library, name);
    if (!pointer) fail(std::string("missing library symbol ") + name);
    return reinterpret_cast<T>(pointer);
}
} // namespace

int main(int argc, char** argv) {
    if (argc < 4 || argc > 7) {
        std::cerr << "usage: native_execution <code.bin> <exh.bin> <translated-library> [entry] [instruction-budget] [r0]\n";
        return 2;
    }
    auto code = readFile(argv[1]);
    auto header = readFile(argv[2]);
    void* library = dlopen(argv[3], RTLD_NOW | RTLD_LOCAL);
    if (!library) fail(dlerror());
    if (*symbol<const std::uint32_t*>(library, "recomp_abi") != RECOMP_ABI) fail("translated ABI disagreement");
    Machine machine{{}, symbol<const Entry*>(library, "recomp_entries"),
                    *symbol<const std::uint32_t*>(library, "recomp_entry_count")};
    for (std::uint32_t i = 1; i < machine.count; ++i) {
        if (machine.entries[i - 1].address >= machine.entries[i].address) fail("entry table is not strictly sorted");
    }
    std::size_t offset = 0;
    for (std::size_t at : {0x10, 0x20, 0x30}) {
        const auto base = word(header, at), pages = word(header, at + 4), size = word(header, at + 8);
        const auto capacity = std::uint64_t(pages) * 4096;
        const auto extent = size + std::uint64_t(at == 0x30 ? word(header, 0x3C) : 0);
        if (size > capacity || offset + capacity > code.size() || base % 4096 || base + extent > UINT32_MAX)
            fail("invalid executable segment extent");
        Region region{base, std::vector<std::uint8_t>((std::max(capacity, extent) + 4095) & ~std::uint64_t(4095)), at == 0x30};
        std::copy_n(code.data() + offset, size, region.bytes.data());
        machine.regions.push_back(std::move(region));
        offset += capacity;
    }
    if (offset != code.size()) fail("executable length disagrees with header");
    machine.regions.push_back({0x0FF00000, std::vector<std::uint8_t>(0x100000), true});
    machine.regions.push_back({0x1FF82000, std::vector<std::uint8_t>(4096), true});
    std::vector<std::uint8_t*> readable(1 << 20), writable(1 << 20);
    for (auto& region : machine.regions) {
        for (std::size_t i = 0; i < region.bytes.size(); i += 4096) {
            const auto page = (region.base + i) >> 12;
            if (readable[page]) fail("overlapping guest regions");
            readable[page] = region.bytes.data() + i;
            if (region.writable) writable[page] = region.bytes.data() + i;
        }
    }
    std::uint32_t vfp[32]{}, fpscr = 0;
    Context context{};
    context.host = &host;
    context.user = &machine;
    context.read_pages = readable.data();
    context.write_pages = writable.data();
    context.vfp = vfp;
    context.fpscr = &fpscr;
    context.tls = 0x1FF82000;
    const auto entry = argc > 4 ? std::stoul(argv[4], nullptr, 0) : word(header, 0x10);
    const auto budget = argc > 5 ? std::stoul(argv[5], nullptr, 0) : 1000000;
    if (entry > UINT32_MAX || budget > INT32_MAX || !budget) fail("invalid entry or budget");
    context.r[15] = static_cast<std::uint32_t>(entry) & ~1u;
    context.thumb = entry & 1;
    context.r[13] = 0x10000000;
    context.r[14] = 0xFFFF0000;
    context.r[0] = argc > 6 ? static_cast<std::uint32_t>(std::stoul(argv[6], nullptr, 0)) : 0;
    context.budget = static_cast<std::int32_t>(budget);
    while (context.budget > 0 && context.r[15] != 0xFFFF0000) {
        auto function = lookup(&context, context.r[15] | context.thumb);
        if (!function) fail("no static translation at " + std::to_string(context.r[15]));
        const auto before = context.budget;
        context.exit = EXIT_NONE;
        context.depth = 0;
        function(&context);
        if (context.exit == EXIT_SVC || context.exit == EXIT_BUDGET) break;
        if (context.budget == before && context.r[15] != 0xFFFF0000) fail("translated dispatch made no progress");
    }
    std::cout << "{\"instruction_count\":" << budget - context.budget
              << ",\"interpreter_instructions\":0,\"exit\":" << context.exit
              << ",\"svc\":" << context.svc << ",\"registers\":[";
    for (int i = 0; i < 16; ++i) std::cout << (i ? "," : "") << context.r[i];
    std::cout << "],\"gpu_frame_verified\":false}\n";
    dlclose(library);
}
