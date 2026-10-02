// Independent stock observations use the same original sites and zero-filled data.
// This executable runs only linked static translations, never a guest decoder.
#include <algorithm>
#include <array>
#include <cstdio>
#include <dlfcn.h>
#include <limits>
#include <stdexcept>
#include "NativeBlockSchedule.h"
#include "NativeTiming.h"

namespace {
struct Fixture { std::uint32_t address, budget, status; };
constexpr std::array<Fixture, 7> Fixtures{{
    {0x0038D004, 2, 0x10}, {0x0038D004, 1, 0x40000010},
    {0x00101B28, 8, 0x10}, {0x00105204, 3, 0x10}, {0x001007C8, 10, 0x10},
    {0x0038D014, 1, 0x40000010}, {0x0038D014, 1, 0x10}}};
struct Probe {
    Port::NativeBlockSchedule schedule;
    const Entry* entries;
    std::uint32_t entry_count;
    std::int64_t remaining;
    explicit Probe(const char* table, const Entry* entries_, std::uint32_t count, std::uint32_t budget)
        : schedule(table), entries(entries_), entry_count(count), remaining(budget) {}
};
Probe& probe(Context* context) { return *static_cast<Probe*>(context->user); }
std::uint8_t read8(Context*, std::uint32_t) { return 0; }
std::uint16_t read16(Context*, std::uint32_t) { return 0; }
std::uint32_t read32(Context*, std::uint32_t) { return 0; }
void write8(Context*, std::uint32_t, std::uint8_t) {}
void write16(Context*, std::uint32_t, std::uint16_t) {}
void write32(Context*, std::uint32_t, std::uint32_t) {}
void refuse(Context*, std::uint32_t, std::uint32_t) { throw std::runtime_error("native probe fallback"); }
void stopDispatch(Context* context) { context->exit = EXIT_BUDGET; }
Code lookup(Context* context, std::uint32_t address) {
    auto& state = probe(context);
    if (!state.schedule.ResolveBoundary(*context, address, state.remaining, false)) return stopDispatch;
    const auto end = state.entries + state.entry_count;
    const auto found = std::lower_bound(state.entries, end, address,
        [](const Entry& entry, std::uint32_t value) { return entry.address < value; });
    if (found == end || found->address != address) throw std::runtime_error("missing native probe entry");
    return found->code;
}
void timing(Context* context, std::uint32_t address, std::uint32_t count, std::uint64_t) {
    auto& state = probe(context);
    state.schedule.BeforeInstruction(*context, address, count, state.remaining, false);
}
const Host Callbacks{read8, read16, read32, write8, write16, write32, refuse, lookup};
template<class T> T symbol(void* library, const char* name) {
    const auto value = dlsym(library, name);
    if (!value) throw std::runtime_error("missing native probe symbol");
    return reinterpret_cast<T>(value);
}
}
int main(int argc, char** argv) {
    try {
        if (argc != 3) throw std::runtime_error("usage: verify_native_block_scheduling translated-library block-schedule");
        void* library = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
        if (!library) throw std::runtime_error(dlerror());
        if (*symbol<const std::uint32_t*>(library, "native_timing_revision") != 2)
            throw std::runtime_error("native probe timing revision differs");
        *symbol<NativeBlockTimingCallback*>(library, "native_block_timing_callback") = timing;
        const auto entries = symbol<const Entry*>(library, "recomp_entries");
        const auto entry_count = *symbol<const std::uint32_t*>(library, "recomp_entry_count");
        for (const auto& fixture : Fixtures) {
            Probe state(argv[2], entries, entry_count, fixture.budget);
            std::array<std::uint32_t, 64> registers{};
            std::vector<std::uint8_t*> pages(1 << 20, nullptr);
            std::uint32_t fpscr = 0x03C00000;
            Context context{};
            context.host = &Callbacks; context.user = &state; context.vfp = registers.data(); context.fpscr = &fpscr;
            context.read_pages = context.write_pages = pages.data(); context.r[15] = fixture.address;
            context.n = fixture.status >> 31; context.z = (fixture.status >> 30) & 1;
            context.c = (fixture.status >> 29) & 1; context.v = (fixture.status >> 28) & 1;
            context.budget = std::numeric_limits<std::int32_t>::max();
            state.schedule.BeginRun();
            std::uint64_t charged = 0, supervisor_prior_charge = 0;
            unsigned supervisors = 0;
            for (;;) {
                context.exit = EXIT_NONE; context.depth = 0;
                lookup(&context, context.r[15] | context.thumb)(&context);
                if (context.exit == EXIT_SVC) {
                    const auto ticks = state.schedule.TakePendingTicks();
                    charged += ticks; state.remaining -= ticks;
                    ++supervisors; supervisor_prior_charge = charged;
                    if (!state.schedule.Complete(context, context.r[15] | context.thumb, state.remaining, false)) break;
                }
                if (context.exit == EXIT_BUDGET) break;
                if (state.schedule.Instructions() > 100000) throw std::runtime_error("native timing probe did not stop");
            }
            charged += state.schedule.TakePendingTicks();
            std::printf("{\"address\":%u,\"budget\":%u,\"initial_status\":%u,\"next_pc\":%u,"
                        "\"charged_ticks\":%llu,\"supervisors\":%u,\"supervisor_prior_charge\":%llu,\"fallbacks\":0}\n",
                        fixture.address, fixture.budget, fixture.status, context.r[15],
                        static_cast<unsigned long long>(charged), supervisors,
                        static_cast<unsigned long long>(supervisor_prior_charge));
        }
        dlclose(library);
    } catch (const std::exception& error) { std::fprintf(stderr, "%s\n", error.what()); return 1; }
}
