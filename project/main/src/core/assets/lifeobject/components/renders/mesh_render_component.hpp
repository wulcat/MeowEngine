//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef PHYSICAT_MESH_RENDER_COMPONENT_HPP
#define PHYSICAT_MESH_RENDER_COMPONENT_HPP

#include "render_component_base.hpp"
#include "static_mesh_instance.hpp"

namespace physicat::core::component {
    class MeshRenderComponent : public physicat::core::component::RenderComponentBase { ;

    public:
        explicit MeshRenderComponent(physicat::assets::ShaderPipelineType shader, physicat::StaticMeshInstance* meshInstance);

//        void Update(const glm::mat4 &projectionViewMatrix) override {
//            MeshInstance->Update(projectionViewMatrix);
//            MeshInstance->RotateBy(0.1f);
//        }

        const physicat::StaticMeshInstance& GetMeshInstance() const {
            return *MeshInstance;
        }

    private:
        physicat::StaticMeshInstance* MeshInstance;
    };
}


#endif //PHYSICAT_MESH_RENDER_COMPONENT_HPP
