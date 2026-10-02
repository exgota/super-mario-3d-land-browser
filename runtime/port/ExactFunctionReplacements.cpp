// Compile the unchanged, byte-exact source with the native compiler.
// The port builder enables this address only while frozen main has rank O.
#include <cstdint>
#include "recomp.h"

using s32 = std::int32_t;
#include "lib/CtrSDK/sources/os_Priority.cpp"

extern "C" RECOMP_OVERRIDE(0x0010766C) {
    ctx->r[0] = static_cast<std::uint32_t>(nn::os::detail::ConvertSvcToLibraryPriority(
        static_cast<std::int32_t>(ctx->r[0])));
    RETURN_TO(ctx->r[14]);
}
