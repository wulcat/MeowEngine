//
// Created by Akira Lynn on 11/09/22.
//

#ifndef PHYSICAT_OPENGL_PIPELINE_HPP
#define PHYSICAT_OPENGL_PIPELINE_HPP

//#include <opengl_texture.hpp>
#include "src/core/wrappers/glm_wrapper.hpp"
#include "src/core/wrappers/graphics_wrapper.hpp"
#include "src/core/internal_ptr.hpp"
//#include "../../application/opengl/opengl_mesh.hpp"
#include "static_mesh_instance.hpp"
#include "opengl_line_pipeline.hpp"
#include "string"
#include "vector"
#include "render_component_base.hpp"

namespace physicat {
    struct OpenGLAssetManager;

    struct OpenGLPipeline {
        OpenGLPipeline(const std::string& shaderName);

    public:
        // Optimization Techniques: https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
        void Render(
            const physicat::OpenGLAssetManager& assetManager,
            const std::vector<physicat::StaticMeshInstance>& staticMeshInstances
        ) const;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
} // namespace physicat


#endif //PHYSICAT_OPENGL_PIPELINE_HPP
