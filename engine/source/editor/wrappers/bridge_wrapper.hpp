//
// Created by Akira Mujawar on 08/07/24.
//

#ifndef MEOWENGINE_BRIDGE_WRAPPER_HPP
#define MEOWENGINE_BRIDGE_WRAPPER_HPP

#ifdef __APPLE__
    extern "C" {
        #include "mac_bridge.h"
    }
#elif _WIN32
    #include <window_bridge.h>

    void createWindowsMenu(HWND hwnd);
#endif

#endif //MEOWENGINE_BRIDGE_WRAPPER_HPP
