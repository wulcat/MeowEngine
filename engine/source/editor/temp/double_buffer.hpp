//
// Created by Akira Mujawar on 29/10/24.
//

#ifndef PHYSICAT_BUFFER_HPP
#define PHYSICAT_BUFFER_HPP

#include "utility"

using namespace std;

namespace physicat {
    template <typename T>
    class DoubleBuffer {
    public:
        DoubleBuffer() : Front{}, Back{} {}
//        DoubleBuffer(T front, T back);

        void Swap() {
            std::swap(Front, Back);
        }

        T& GetFront() {
            return Front;
        }

        T& GetBack() {
            return Back;
        }

    private:
        T Front;
        T Back;
    };
}


#endif //PHYSICAT_BUFFER_HPP
