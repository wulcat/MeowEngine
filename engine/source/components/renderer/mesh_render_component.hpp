//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef MEOWENGINE_MESH_RENDER_COMPONENT_HPP
#define MEOWENGINE_MESH_RENDER_COMPONENT_HPP

#include "render_component_base.hpp"
#include "static_mesh_instance.hpp"

namespace MeowEngine::entity {
    class DummyClass {
    public:
        static void Reflect();

        int MeshType;
        float Size;
    };

    class MeshRenderComponent : public MeowEngine::entity::RenderComponentBase {

    public:
        static void Reflect();

        explicit MeshRenderComponent(MeowEngine::assets::ShaderPipelineType shader, MeowEngine::StaticMeshInstance* meshInstance);

//        void Update(const glm::mat4 &projectionViewMatrix) override {
//            MeshInstance->Update(projectionViewMatrix);
//            MeshInstance->RotateBy(0.1f);
//        }

        const MeowEngine::StaticMeshInstance& GetMeshInstance() const {
            return *MeshInstance;
        }

        DummyClass Data;

    private:
        MeowEngine::StaticMeshInstance* MeshInstance;
    };
}


#endif //MEOWENGINE_MESH_RENDER_COMPONENT_HPP
