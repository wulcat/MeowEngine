//
// Created by Akira Mujawar on 05/01/25.
//

#ifndef PHYSICAT_ENTT_BUFFER_HPP
#define PHYSICAT_ENTT_BUFFER_HPP

#include "entt_wrapper.hpp"
#include "double_buffer.hpp"
#include "queue"
#include "functional"
#include "concurrentqueue.h"

#include <transform3d_component.hpp>
#include <rigidbody_component.hpp>
#include <collider_component.hpp>

#include "physics.hpp"

using namespace std;

namespace MeowEngine {
    struct EnttBuffer : public DoubleBuffer<entt::registry> {
    public:
        EnttBuffer();

        entt::entity Create();

        entt::registry& GetStaging();

        template<typename ComponentType, typename... Args>
        void AddComponent(entt::entity& inEntity, Args &&...inArgs);

        template<typename ComponentType, typename... Args>
        void AddComponent(const entt::entity& inEntity, Args &&...inArgs);

        void CreateInStaging() {
            entt::entity entity;
            while(StagingCreation.try_dequeue(entity)) {
//                entt::entity entity = StagingCreation.front();
                Staging.create(entity);

//                StagingCreation.pop();
            }
        }
        void AddInStaging(MeowEngine::simulator::Physics* inPhysics) {
//            do {
                std::function<void(MeowEngine::simulator::Physics*)> method;
                while(Test.try_dequeue(method)) {
                    method(inPhysics);


                }
//            } while(true);


//            while(!Test.empty()) {
//                Test.front()();
//                Test.pop();
//            }
        }

    protected:
        entt::registry Staging;

    private:
        moodycamel::ConcurrentQueue<entt::entity> StagingCreation;
        moodycamel::ConcurrentQueue<std::function<void(MeowEngine::simulator::Physics*)>> Test;
//        std::queue<std::function<void()>> TestM;
    };

    template<typename Type, typename... Args>
    void MeowEngine::EnttBuffer::AddComponent(entt::entity &inEntity, Args &&... inArgs) {
        Current.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
        Final.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);

        Test.enqueue([&, inEntity, inArgTuple = std::make_tuple(std::forward<Args>(inArgs)...)](MeowEngine::simulator::Physics* inPhysics) {
            std::apply([&](auto&&... inUnpacked) {

                Staging.emplace<Type>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);

//                auto [transform, collider, rigidbody] =  Staging.get<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>(inEntity);
//                if(transform != nullptr)

            }, inArgTuple);

            if(Staging.all_of<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>(inEntity)) {
                auto [transform, collider, rigidbody] =  Staging.get<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>(inEntity);
                inPhysics->AddRigidbody(transform, collider, rigidbody);
            }
//            Staging.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
        });
//        TestM.push([=]() {
////            auto testddd = std::forward<Args>(inArgs)...;
//            Staging.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
//        });
    }

    template<typename Type, typename... Args>
    void MeowEngine::EnttBuffer::AddComponent(const entt::entity &inEntity, Args &&... inArgs) {
        Current.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
//        Staging.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
        Final.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);

        Test.enqueue([&, inEntity, inArgTuple = std::make_tuple(std::forward<Args>(inArgs)...)](MeowEngine::simulator::Physics* inPhysics) {
            std::apply([&](auto&&... inUnpacked) {

                Staging.emplace<Type>(inEntity, std::forward<decltype(inUnpacked)>(inUnpacked)...);
            }, inArgTuple);
//            Staging.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
            if(Staging.all_of<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>(inEntity)) {
                auto [transform, collider, rigidbody] =  Staging.get<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>(inEntity);
                inPhysics->AddRigidbody(transform, collider, rigidbody);
            }
        });
    }
}


#endif //PHYSICAT_ENTT_BUFFER_HPP
