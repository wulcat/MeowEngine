//
// Created by Akira Mujawar on 29/10/24.
//

#ifndef MEOWENGINE_BUFFER_HPP
#define MEOWENGINE_BUFFER_HPP

#include "utility"
#include "tracy_wrapper.hpp"

using namespace std;

namespace MeowEngine {
    template <typename T>
    class DoubleBuffer {

    public:
        DoubleBuffer() : Current{}, Final{} {}

        T& GetCurrent() {
            return Current;
        }

        T& GetFinal() {
            return Final;
        }

        void Swap() {
            PT_PROFILE_SCOPE;
            std::swap(Current, Final);
        }

    protected:
        T Current;
        T Final;
    };
}


#endif //MEOWENGINE_BUFFER_HPP
