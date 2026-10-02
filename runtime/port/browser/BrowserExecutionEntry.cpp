// SPDX-License-Identifier: GPL-2.0-or-later
#include "common/logging/backend.h"
#include "BrowserButtonInput.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

int RunStaticCompiledExecution(int argc, char** argv);

int main(int argc, char** argv) {
    const char* requested = std::getenv("ROOT_PORT_BROWSER_BUTTON_CAPTURE");
    if (requested && (std::strcmp(requested, "1") != 0 || argc != 4)) {
        std::cerr << "Browser button recording requires value 1 and no playback movie\n";
        return 2;
    }
    if (requested) Port::InstallBrowserButtonInput();
    const int status = RunStaticCompiledExecution(argc, argv);
    if (requested) Port::UninstallBrowserButtonInput();
    // Join and flush while the outer runtime worker can still service the
    // logger's synchronous file operations, before Emscripten starts exit.
    Common::Log::Stop();
    return status;
}
