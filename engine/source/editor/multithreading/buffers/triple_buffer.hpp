//
// Created by Akira Mujawar on 05/01/25.
//

#ifndef MEOWENGINE_TRIPLE_BUFFER_HPP
#define MEOWENGINE_TRIPLE_BUFFER_HPP

#include "utility"

using namespace std;

namespace MeowEngine {
    template <typename T>
    struct TripleBuffer {

    public:
        TripleBuffer() : Current{}, Staging{}, Final{} {}

        T& GetCurrent() {
            return  Current;
        }

        T& GetStaging() {
            return  Staging;
        }

        T& GetFinal() {
            return  Final;
        }

        void Swap() {
            std::swap(Current, Staging);
            std::swap(Current, Final);
        }

    protected:
        T Current;
        T Staging;
        T Final;
    };
}


#endif //MEOWENGINE_TRIPLE_BUFFER_HPP
