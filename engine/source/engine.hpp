//
// Created by Akira Mujawar on 06/07/22.
//

#ifndef PHYSICAT_ENGINE_HPP
#define PHYSICAT_ENGINE_HPP

#pragma once

#include "internal_ptr.hpp"

namespace MeowEngine {
    struct Engine {
        Engine();

        void Run()  ;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
} // namespace MeowEngine

#endif //PHYSICAT_ENGINE_HPP
