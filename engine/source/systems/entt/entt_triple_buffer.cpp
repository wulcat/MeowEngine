//
// Created by Akira Mujawar on 05/01/25.
//

#include "entt_triple_buffer.hpp"
#include "entt_reflection_wrapper.hpp"

MeowEngine::EnttTripleBuffer::EnttTripleBuffer()
 : Staging() {}

entt::registry& MeowEngine::EnttTripleBuffer::GetCurrent() {
    return DoubleBuffer.GetCurrent();
}

entt::registry& MeowEngine::EnttTripleBuffer::GetFinal() {
    return DoubleBuffer.GetFinal();
}

entt::registry& MeowEngine::EnttTripleBuffer::GetStaging() {
    return Staging;
}

void MeowEngine::EnttTripleBuffer::SwapBuffer() {
    DoubleBuffer.Swap();
}

entt::entity MeowEngine::EnttTripleBuffer::AddEntity() {
    entt::entity entity = DoubleBuffer.GetCurrent().create();
    DoubleBuffer.GetFinal().create(entity);

    EntityToAddOnStagingQueue.enqueue(entity);

    return entity;
}

void MeowEngine::EnttTripleBuffer::RemoveEntity(entt::entity &inEntity) {
    EntityToRemoveOnMainRenderQueue.push(inEntity);
}

void MeowEngine::EnttTripleBuffer::RemoveEntity(const entt::entity &inEntity) {
    EntityToRemoveOnMainRenderQueue.push(inEntity);
}

bool MeowEngine::EnttTripleBuffer::ApplyAddRemoveOnStaging(MeowEngine::simulator::PhysicsSystem* inPhysics) {
    bool isEntityOrComponentChanged = false;

    // If we have anything to dequeue from concurrent queue, we replicate and add in staging buffer
    // Add / Remove entities from staging (physics) buffer - from physics thread
    entt::entity entity;
    while(EntityToAddOnStagingQueue.try_dequeue(entity)) {
        isEntityOrComponentChanged = true;
        Staging.create(entity);
    }

    // Adds components with dynamic data parameter passed down from main thread
    // Add / Remove components from staging (physics) buffer - from physics thread
    std::function<void(MeowEngine::simulator::PhysicsSystem*)> method;
    while(ComponentToAddOnStagingQueue.try_dequeue(method)) {
        isEntityOrComponentChanged = true;
        method(inPhysics);
    }

    // Destroys entities from staging (queued while destroying from current final from main thread)
    entt::entity entityToRemove;
    while(EntityToRemoveOnStagingQueue.try_dequeue(entityToRemove)) {
        isEntityOrComponentChanged = true;
        // only destroy if rigidbody exists
        if(Staging.all_of<entity::RigidbodyComponent>(entityToRemove)) {
            auto rigidbody = Staging.get<entity::RigidbodyComponent>(entityToRemove);
            inPhysics->RemoveRigidbody(rigidbody);
        }

        GetStaging().destroy(entityToRemove);
    }

    return isEntityOrComponentChanged;
}

bool MeowEngine::EnttTripleBuffer::ApplyAddRemoveOnCurrentFinal() {
    // Adds component on current & final buffer through AddComponent cached method.
    // It further queues up to be added for physics
    bool isEntityOrComponentChanged = false;

    while(!ComponentToAddOnMainRenderQueue.empty()) {
        ComponentToAddOnMainRenderQueue.front()();
        ComponentToAddOnMainRenderQueue.pop();

        isEntityOrComponentChanged = true;
    }

    // Destroys entities from current & final buffer and queues for staging
    while(!EntityToRemoveOnMainRenderQueue.empty()) {
        entt::entity entity = EntityToRemoveOnMainRenderQueue.front();
        EntityToRemoveOnMainRenderQueue.pop();

        GetCurrent().destroy(entity);
        GetFinal().destroy(entity);

        EntityToRemoveOnStagingQueue.enqueue(entity);

        isEntityOrComponentChanged = true;
    }

    return isEntityOrComponentChanged;
}

void MeowEngine::EnttTripleBuffer::ApplyPropertyChange() {
    // Apply changes on current and final buffer and push into queue for same changes for staging
    while(!UiInputPropertyChangesQueue.empty()) {
        std::shared_ptr<MeowEngine::ReflectionPropertyChange> change = UiInputPropertyChangesQueue.front();

        MeowEngine::Reflection.ApplyPropertyChange(*change, DoubleBuffer.GetCurrent());
        MeowEngine::Reflection.ApplyPropertyChange(*change, DoubleBuffer.GetFinal());

        PhysicsUiInputPropertyChangesQueue.enqueue(change);
        UiInputPropertyChangesQueue.pop();
    }
}

void MeowEngine::EnttTripleBuffer::ApplyPropertyChangeOnStaging() {
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