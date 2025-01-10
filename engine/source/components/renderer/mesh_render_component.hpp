//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef PHYSICAT_MESH_RENDER_COMPONENT_HPP
#define PHYSICAT_MESH_RENDER_COMPONENT_HPP

#include "render_component_base.hpp"
#include "static_mesh_instance.hpp"

namespace physicat::entity {
    class DummyClass {
    public:
        static void Reflect();

        int MeshType;
        float Size;
    };

    class MeshRenderComponent : public physicat::entity::RenderComponentBase { ;

    public:
        static void Reflect();

        explicit MeshRenderComponent(physicat::assets::ShaderPipelineType shader, physicat::StaticMeshInstance* meshInstance);

//        void Update(const glm::mat4 &projectionViewMatrix) override {
//            MeshInstance->Update(projectionViewMatrix);
//            MeshInstance->RotateBy(0.1f);
//        }

        const physicat::StaticMeshInstance& GetMeshInstance() const {
            return *MeshInstance;
        }

        DummyClass Data;

    private:
        physicat::StaticMeshInstance* MeshInstance;
    };
}


#endif //PHYSICAT_MESH_RENDER_COMPONENT_HPP
