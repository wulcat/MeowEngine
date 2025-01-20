//
// Created by Akira Mujawar on 09/01/25.
//

#ifndef MEOWENGINE_PSTRING_HPP
#define MEOWENGINE_PSTRING_HPP

#include "string"

namespace MeowEngine {
    class PString : public std::string {

    public:
        static void Reflect();

        PString();
        PString(std::string inString);

        template<typename... Args>
        static std::string Format(const char* fmt, Args... args)
        {
            size_t size = snprintf(nullptr, 0, fmt, args...);
            std::string buf;
            buf.reserve(size + 1);
            buf.resize(size);
            snprintf(&buf[0], size + 1, fmt, args...);
            return buf;
        }
    };
}

#endif //MEOWENGINE_PSTRING_HPP
