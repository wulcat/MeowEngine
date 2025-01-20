//
// Created by Akira Mujawar on 05/01/25.
//

#include "entt_buffer.hpp"
#include "entt_reflection_wrapper.hpp"

MeowEngine::EnttBuffer::EnttBuffer()
 : Staging() {}

entt::registry& MeowEngine::EnttBuffer::GetStaging() {
    return Staging;
}

std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>>& MeowEngine::EnttBuffer::GetPropertyChangeQueue() {
    return UiInputPropertyChangesQueue;
}

entt::entity MeowEngine::EnttBuffer::AddEntity() {
    entt::entity entity = Current.create();
    Final.create(entity);

    EntityToAddOnStagingQueue.enqueue(entity);

    return entity;
}

void MeowEngine::EnttBuffer::ApplyAddRemoveOnStaging(MeowEngine::simulator::Physics* inPhysics) {
    AddEntitiesOnStaging();
    AddComponentsOnStaging(inPhysics);
}

void MeowEngine::EnttBuffer::AddEntitiesOnStaging() {
    // If we have anything to dequeue from concurrent queue, we replicate and add in staging buffer
    entt::entity entity;
    while(EntityToAddOnStagingQueue.try_dequeue(entity)) {
        Staging.create(entity);
    }
}

void MeowEngine::EnttBuffer::AddComponentsOnStaging(MeowEngine::simulator::Physics* inPhysics) {
    // Adds components with dynamic data parameter passed down from main thread
    std::function<void(MeowEngine::simulator::Physics*)> method;
    while(ComponentToAddOnStagingQueue.try_dequeue(method)) {
        method(inPhysics);
    }
}

void MeowEngine::EnttBuffer::ApplyPropertyChange() {
    // Apply changes on current and final buffer and push into queue for same changes for staging
    while(!UiInputPropertyChangesQueue.empty()) {
        std::shared_ptr<MeowEngine::ReflectionPropertyChange> change = UiInputPropertyChangesQueue.front();

        MeowEngine::Reflection.ApplyPropertyChange(*change.get(), Current);
        MeowEngine::Reflection.ApplyPropertyChange(*change.get(), Final);

        PhysicsUiInputPropertyChangesQueue.enqueue(change);
        UiInputPropertyChangesQueue.pop();
    }
}

void MeowEngine::EnttBuffer::ApplyPropertyChangeOnStaging() {
    // Apply update physics transform to entities
    auto view = Staging.view<entity::Transform3DComponent, entity::RigidbodyComponent>();

    // Apply UI inputs to physics components
    std::shared_ptr<MeowEngine::ReflectionPropertyChange> change;
    while(PhysicsUiInputPropertyChangesQueue.try_dequeue(change)) {
        if(view.contains(static_cast<entt::entity>(change->EntityId))) {
            MeowEngine::Reflection.ApplyPropertyChange(*change, Staging);
            auto [transform, rigidbody] = view.get<entity::Transform3DComponent, entity::RigidbodyComponent>(static_cast<entt::entity>(change->EntityId));
            rigidbody.OverrideTransform(transform);
        }
    }
}