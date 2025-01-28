//
// Created by Akira Mujawar on 27/01/25.
//

#ifndef MEOWENGINE_ENTT_BUFFER_BASE_HPP
#define MEOWENGINE_ENTT_BUFFER_BASE_HPP

#include "entt_wrapper.hpp"

#include "queue"
#include "reflection_property_change.hpp"

namespace MeowEngine {
    struct EnttBufferBase {
        EnttBufferBase() = default;
        virtual ~EnttBufferBase() = default;

        /**
         * We pass this to UI render system, when a change is made it gets pushed to this queue
         * Which are applied at the end of frame at sync point both single / multi threads
         * @return
         */
        std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>>& GetPropertyChangeQueue()
        {
            return UiInputPropertyChangesQueue;
        }

        virtual entt::registry& GetCurrent() = 0;

        /**
         * Creates a empty entity
         * @return
         */
        virtual entt::entity AddEntity() = 0;

        /**
         * Single-thread: Changes are applied when frame finishes
         * Multi-thread:  Any queued property value changes are applied to current(main) & final(render) buffers.
         * Further we queue them for staging(physics)
         */
        virtual void ApplyPropertyChange() = 0;

    protected:
        /**
         * When a property value is changed on Render (ui) we queue in this list
         */
        std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>> UiInputPropertyChangesQueue;
    };

}

#endif //MEOWENGINE_ENTT_BUFFER_BASE_HPP
