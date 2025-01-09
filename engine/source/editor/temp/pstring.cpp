//
// Created by Akira Mujawar on 09/01/25.
//

#include "pstring.hpp"
#include "log.hpp"

void physicat::PString::Reflect() {
    physicat::Log("Reflected", "PString");
}

physicat::PString::PString() {

}

physicat::PString::PString(std::string inString) {
    this->assign(inString);
}