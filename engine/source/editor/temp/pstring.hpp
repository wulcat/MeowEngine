//
// Created by Akira Mujawar on 09/01/25.
//

#ifndef PHYSICAT_PSTRING_HPP
#define PHYSICAT_PSTRING_HPP

#include "string"

namespace physicat {
    class PString : public std::string {

    public:
        static void Reflect();

        PString();
        PString(std::string inString);
    };
}

#endif //PHYSICAT_PSTRING_HPP
