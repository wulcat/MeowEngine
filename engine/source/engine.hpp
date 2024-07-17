//
// Created by Akira Lynn on 06/07/22.
//

#ifndef PHYSICAT_ENGINE_HPP
#define PHYSICAT_ENGINE_HPP

#pragma once

#include "internal_ptr.hpp"

namespace physicat {
    struct Engine {
        Engine();

        void Run()  ;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
} // namespace physicat

#endif //PHYSICAT_ENGINE_HPP
