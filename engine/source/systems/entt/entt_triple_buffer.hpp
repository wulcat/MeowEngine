//
// Created by Akira Mujawar on 05/01/25.
//

#ifndef MEOWENGINE_ENTT_TRIPLE_BUFFER_HPP
#define MEOWENGINE_ENTT_TRIPLE_BUFFER_HPP

#include "entt_wrapper.hpp"
#include "double_buffer.hpp"
#include "queue"
#include "functional"
#include "concurrentqueue.h"

#include <transform3d_component.hpp>
#include <rigidbody_component.hpp>
#include <collider_component.hpp>
#include "reflection_property_change.hpp"
#include "physics_system.hpp"

#include "entt_buffer_base.hpp"

using namespace std;

// entt_wrapper
//

// entt
// entt_system - pure class?
// - Add Entity
// - Add Component - cannot be virtual >.<
// - Reflection

// entt_single_buffer_system
// implement

// entt_triple_buffer_system
// implement

// entt_single_thread_system
// entt_multi_thread_system

// entt double buffer

namespace MeowEngine {
    struct EnttTripleBuffer : public MeowEngine::EnttBufferBase {
    public:
        EnttTripleBuffer();

        entt::registry& GetCurrent() override;
        entt::registry& GetFinal();
        entt::registry& GetStaging();

        void SwapBuffer();

//        std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>>& GetPropertyChangeQueue();

        entt::entity AddEntity() override;

        template<typename ComponentType, typename... Args>
        void AddComponent(entt::entity& inEntity, Args &&...inArgs);

        template<typename ComponentType, typename... Args>
        void AddComponent(const entt::entity& inEntity, Args &&...inArgs);

        /**
         * Add / Remove entities & components on staging buffer which are queued from main thread
         * @param inPhysics
         */
        void ApplyAddRemoveOnStaging(MeowEngine::simulator::PhysicsSystem* inPhysics);

        /**
         * Any queued property value changes are applied to current(main) & final(render) buffers.
         * Further we queue them for staging(physics)
         */
        void ApplyPropertyChange() override;

        /**
         * Any queued property value changes are applied to staging(physics) buffers.
         */
        void ApplyPropertyChangeOnStaging();

    protected:
        /**
         * Add / Remove entities from staging (physics) buffer
         */
        void AddEntitiesOnStaging();

        /**
         * Add / Remove components from staging (physics) buffer
         * @param inPhysics
         */
        void AddComponentsOnStaging(MeowEngine::simulator::PhysicsSystem* inPhysics);

        MeowEngine::DoubleBuffer<entt::registry> DoubleBuffer;
        entt::registry Staging;

        /**
         * When a property value is changed on Render (ui) we queue in this list
         */
//        std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>> UiInputPropertyChangesQueue;

    private:
        /**
         * When a entity is added on main thread, we queue it and add it async on physics thread
         */
        moodycamel::ConcurrentQueue<entt::entity> EntityToAddOnStagingQueue;

        /**
         * When a component is added from main thread, we queue it as a method with args
         * and add it async on physics thread
         */
        moodycamel::ConcurrentQueue<std::function<void(MeowEngine::simulator::PhysicsSystem*)>> ComponentToAddOnStagingQueue;

        /**
         * Any queued property value changes are applied to staging(physics) buffer
         */
        moodycamel::ConcurrentQueue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>> PhysicsUiInputPropertyChangesQueue;
    };

    template<typename ComponentType, typename... Args>
    void MeowEngine::EnttTripleBuffer::AddComponent(entt::entity &inEntity, Args &&... inArgs) {
        DoubleBuffer.GetCurrent().emplace<ComponentType>(inEntity, std::forward<Args>(inArgs)...);
        DoubleBuffer.GetFinal().emplace<ComponentType>(inEntity, std::forward<Args>(inArgs)...);

        ComponentToAddOnStagingQueue.enqueue([&, inEntity, inArgTuple = std::make_tuple(std::forward<Args>(inArgs)...)](MeowEngine::simulator::PhysicsSystem* inPhysics) {
            std::apply([&](auto&&... inUnpacked) {

                Staging.emplace<ComponentType>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);
            }, inArgTuple);

            if(Staging.all_of<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>(inEntity)) {
                auto [transform, collider, rigidbody] =  Staging.get<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>(inEntity);
                inPhysics->AddRigidbody(transform, collider, rigidbody);
            }
        });

    }

    template<typename ComponentType, typename... Args>
    void MeowEngine::EnttTripleBuffer::AddComponent(const entt::entity &inEntity, Args &&... inArgs) {
        DoubleBuffer.GetCurrent().emplace<ComponentType>(inEntity, std::forward<Args>(inArgs)...);
        DoubleBuffer.GetFinal().emplace<ComponentType>(inEntity, std::forward<Args>(inArgs)...);

        ComponentToAddOnStagingQueue.enqueue([&, inEntity, inArgTuple = std::make_tuple(std::forward<Args>(inArgs)...)](MeowEngine::simulator::PhysicsSystem* inPhysics) {
            std::apply([&](auto&&... inUnpacked) {

                Staging.emplace<ComponentType>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);
            }, inArgTuple);
            if(Staging.all_of<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>(inEntity)) {
                auto [transform, collider, rigidbody] =  Staging.get<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>(inEntity);
                inPhysics->AddRigidbody(transform, collider, rigidbody);
            }
        });
    }
}


#endif //MEOWENGINE_ENTT_TRIPLE_BUFFER_HPP
