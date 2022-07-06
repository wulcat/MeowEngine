//
// Created by Akira Lynn on 06/07/22.
//

#ifndef PHYSICAT_ENGINE_HPP
#define PHYSICAT_ENGINE_HPP

#pragma once

#include "core/internal_ptr.hpp"

namespace physicat {
    struct Engine {
        Engine();

        void Begin();

    private:
        struct Internal;
        physicat::internal_ptr<Internal> internal;
    };
} // namespace physicat

#endif //PHYSICAT_ENGINE_HPP
