// SPDX-License-Identifier: GPL-2.0-or-later
#include "common/logging/backend.h"

int RunStaticCompiledExecution(int argc, char** argv);

int main(int argc, char** argv) {
    const int status = RunStaticCompiledExecution(argc, argv);
    // Join and flush while the outer runtime worker can still service the
    // logger's synchronous file operations, before Emscripten starts exit.
    Common::Log::Stop();
    return status;
}
