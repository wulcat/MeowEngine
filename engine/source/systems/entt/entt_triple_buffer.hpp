//
// Created by Akira Mujawar on 05/01/25.
//

#ifndef MEOWENGINE_ENTT_TRIPLE_BUFFER_HPP
#define MEOWENGINE_ENTT_TRIPLE_BUFFER_HPP

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


namespace MeowEngine {
    struct EnttTripleBuffer : public MeowEngine::EnttBufferBase {
    public:
        EnttTripleBuffer();

        entt::registry& GetCurrent() override;
        entt::registry& GetFinal();
        entt::registry& GetStaging();

        void SwapBuffer();

        entt::entity AddEntity() override;
        void RemoveEntity(entt::entity& inEntity);
        void RemoveEntity(const entt::entity& inEntity);

        /**
         * Registers components to keep the storage in order,
         * that way when we swap buffers we get the fixed order of component
         * @tparam ComponentType
         */
        template<typename ComponentType>
        void RegisterComponent();

        /**
         * Can be called from any where in main thread.
         * Queues up to add components on current, final & staging
         * @tparam ComponentType
         * @tparam Args
         * @param inEntity
         * @param inArgs
         */
        template<typename ComponentType, typename... Args>
        void AddComponent(entt::entity& inEntity, Args &&...inArgs);

        /**
         * Can be called from any where in main thread.
         * Queues up to add components on current, final & staging
         * @tparam ComponentType
         * @tparam Args
         * @param inEntity
         * @param inArgs
         */
        template<typename ComponentType, typename... Args>
        void AddComponent(const entt::entity& inEntity, Args &&...inArgs);

        template<typename ComponentType>
        void RemoveComponent(entt::entity& inEntity);

        template<typename ComponentType>
        void RemoveComponent(const entt::entity& inEntity);

        /**
         * At sync point, we add any pending requests for adding components to final & current buffers
         * - done from main thread
         */
        void ApplyAddRemoveOnCurrentFinal();

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
         * Add / Remove entities from staging (physics) buffer - from physics thread
         */
        void AddEntitiesOnStaging();

        /**
         * Add / Remove components from staging (physics) buffer - from physics thread
         * @param inPhysics
         */
        void AddComponentsOnStaging(MeowEngine::simulator::PhysicsSystem* inPhysics);

        /**
         * Adds component to staging (physics) buffer after its been added to current/final successfully
         * - from main thread
         * @tparam ComponentType
         * @tparam Args
         * @param inEntity
         * @param inArgs
         */
        template<typename ComponentType, typename... Args>
        void AddComponentOnStaging(entt::entity& inEntity, Args &&...inArgs);

        /**
         * Adds component to staging (physics) buffer after its been added to current/final successfully
         * - from main thread
         * @tparam ComponentType
         * @tparam Args
         * @param inEntity
         * @param inArgs
         */
        template<typename ComponentType, typename... Args>
        void AddComponentOnStaging(const entt::entity& inEntity, Args &&...inArgs);

        MeowEngine::DoubleBuffer<entt::registry> DoubleBuffer;
        entt::registry Staging;

    private:
        //-------- Adding

        /**
         * When a entity is added on main thread, we queue it and add it async on physics thread
         */
        moodycamel::ConcurrentQueue<entt::entity> EntityToAddOnStagingQueue;

        /**
         * Entities are added right away to both current & final. But components are added in queue and added once
         * both main & render thread at sync point
         */
        std::queue<std::function<void()>> ComponentToAddOnMainRenderQueue;

        /**
         * When a component is added from main thread, we queue it as a method with args
         * and add it async on physics thread
         */
        moodycamel::ConcurrentQueue<std::function<void(MeowEngine::simulator::PhysicsSystem*)>> ComponentToAddOnStagingQueue;

        //-------- Removal

        std::queue<entt::entity> EntityToRemoveOnMainRenderQueue;
        moodycamel::ConcurrentQueue<entt::entity> EntityToRemoveOnStagingQueue;

        //-------- Property Changes
        /**
         * Any queued property value changes are applied to staging(physics) buffer
         */
        moodycamel::ConcurrentQueue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>> PhysicsUiInputPropertyChangesQueue;
    };

    template<typename ComponentType>
    void MeowEngine::EnttTripleBuffer::RegisterComponent() {
        DoubleBuffer.GetCurrent().storage<ComponentType>();
        DoubleBuffer.GetFinal().storage<ComponentType>();
    }

    template<typename ComponentType, typename... Args>
    void MeowEngine::EnttTripleBuffer::AddComponent(entt::entity &inEntity, Args &&... inArgs) {
        ComponentToAddOnMainRenderQueue.push([&, inEntity, inArgTuple = std::make_tuple(std::forward<Args>(inArgs)...)]() {
            std::apply([&](auto&&... inUnpacked) {
                DoubleBuffer.GetCurrent().emplace<ComponentType>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);
                DoubleBuffer.GetFinal().emplace<ComponentType>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);

                AddComponentOnStaging<ComponentType>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);
            }, inArgTuple);
        });
    }

    template<typename ComponentType, typename... Args>
    void MeowEngine::EnttTripleBuffer::AddComponent(const entt::entity &inEntity, Args &&... inArgs) {
        ComponentToAddOnMainRenderQueue.push([&, inEntity, inArgTuple = std::make_tuple(std::forward<Args>(inArgs)...)]() {
            std::apply([&](auto&&... inUnpacked) {
                DoubleBuffer.GetCurrent().emplace<ComponentType>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);
                DoubleBuffer.GetFinal().emplace<ComponentType>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);

                AddComponentOnStaging<ComponentType>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);
            }, inArgTuple);
        });
    }

    template<typename ComponentType, typename... Args>
    void MeowEngine::EnttTripleBuffer::AddComponentOnStaging(const entt::entity &inEntity, Args &&... inArgs) {
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
    void MeowEngine::EnttTripleBuffer::AddComponentOnStaging(entt::entity &inEntity, Args &&... inArgs) {
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

    template<typename ComponentType>
    void MeowEngine::EnttTripleBuffer::RemoveComponent(entt::entity &inEntity) {}

    template<typename ComponentType>
    void MeowEngine::EnttTripleBuffer::RemoveComponent(const entt::entity &inEntity) {}
}


#endif //MEOWENGINE_ENTT_TRIPLE_BUFFER_HPP
