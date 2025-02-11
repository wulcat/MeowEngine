//
// Created by Akira Mujawar on 11/02/25.
//

#ifndef MEOWENGINE_SCENE_ADD_REMOVE_ENTITY_HPP
#define MEOWENGINE_SCENE_ADD_REMOVE_ENTITY_HPP

#include "scene_multi_thread.hpp"

namespace MeowEngine {

    struct SceneAddRemoveEntity : SceneMultiThread {
        void Update(const float &deltaTime) override;

    private:
        float test = 0;
        float test2 = 0;
    };

} // MeowEngine

#endif //MEOWENGINE_SCENE_ADD_REMOVE_ENTITY_HPP
