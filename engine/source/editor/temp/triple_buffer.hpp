//
// Created by Akira Mujawar on 05/01/25.
//

#ifndef PHYSICAT_TRIPLE_BUFFER_HPP
#define PHYSICAT_TRIPLE_BUFFER_HPP

using namespace std;

namespace physicat {
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

    protected:
        T Current;
        T Staging;
        T Final;
    };
}


#endif //PHYSICAT_TRIPLE_BUFFER_HPP
