//
// Created by Akira Lynn on 06/07/22.
//

#ifndef NDEBUG
    #ifdef __ANDROID__
        #include <android/log.h>
    #endif
#endif

#include "log.hpp"
#include "iostream"


void physicat::log(const std::string &tag, const std::string &message) {
#ifndef NDEBUG
    #ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_DEBUG, "a-simple-triangle", "%s: %s", tag.c_str(), message.c_str());
    #else
        std::cout << tag << ": " << message << std::endl;
    #endif
#endif
}

void physicat::log(const std::string &tag, const std::string &message, const std::exception &error) {
#ifndef NDEBUG
    std::string output = message + " Exception message was: " + std::string{error.what()};
    physicat::log(tag, output);
#endif
}
