// SPDX-License-Identifier: GPL-2.0-or-later
#include "common/logging/backend.h"
#include "BrowserButtonInput.h"
#include "BrowserFrameOutput.h"
#include "BrowserCirclePadInput.h"
#include "BrowserTouchInput.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

int RunStaticCompiledExecution(int argc, char** argv);

namespace {
class CirclePadInputScope {
public:
    explicit CirclePadInputScope(bool requested) : installed(!requested || Port::InstallBrowserCirclePadInput()),
                                                   requested(requested) {}
    ~CirclePadInputScope() { if (requested && installed) Port::UninstallBrowserCirclePadInput(); }
    bool Valid() const { return installed; }
private:
    bool installed;
    bool requested;
};
class TouchInputScope {
public:
    explicit TouchInputScope(bool requested) : installed(!requested || Port::InstallBrowserTouchInput()),
                                              requested(requested) {}
    ~TouchInputScope() { if (requested && installed) Port::UninstallBrowserTouchInput(); }
    bool Valid() const { return installed; }
private:
    bool installed;
    bool requested;
};
}

int main(int argc, char** argv) {
    const char* requested = std::getenv("ROOT_PORT_BROWSER_BUTTON_CAPTURE");
    if (requested && (std::strcmp(requested, "1") != 0 || argc != 4)) {
        std::cerr << "Browser button recording requires value 1 and no playback movie\n";
        return 2;
    }
    const char* circle = std::getenv("ROOT_PORT_BROWSER_CIRCLE_PAD_CAPTURE");
    if (circle && (std::strcmp(circle, "1") != 0 || argc != 4)) {
        std::cerr << "Browser circle-pad recording requires value 1 and no playback movie\n";
        return 2;
    }
    const char* frames = std::getenv("ROOT_PORT_BROWSER_FRAME_OUTPUT");
    const char* touch = std::getenv("ROOT_PORT_BROWSER_TOUCH_CAPTURE");
    if (touch && (std::strcmp(touch, "1") != 0 || argc != 4)) {
        std::cerr << "Browser touch recording requires value 1 and no playback movie\n";
        return 2;
    }
    if (frames && (std::strcmp(frames, "1") != 0 || (argc != 4 && argc != 6))) {
        std::cerr << "Browser frame output requires value 1 and a finite capture\n";
        return 2;
    }
    if (frames) Port::StartBrowserFrameOutput();
    if (requested || frames || circle || touch) Port::InstallBrowserButtonInput(requested != nullptr);
    int status;
    {
        CirclePadInputScope circle_scope(circle != nullptr);
        TouchInputScope touch_scope(touch != nullptr);
        status = circle_scope.Valid() && touch_scope.Valid() ? RunStaticCompiledExecution(argc, argv) : 2;
    }
    if (requested || frames || circle || touch) Port::UninstallBrowserButtonInput();
    if (frames) Port::StopBrowserFrameOutput();
    // Join and flush while the outer runtime worker can still service the
    // logger's synchronous file operations, before Emscripten starts exit.
    Common::Log::Stop();
    return status;
}
