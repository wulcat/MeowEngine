//
// Created by Akira Mujawar on 03/07/24.
//

uniform mat4 u_view;
uniform mat4 u_projection;
//uniform vec3 position;

out vec3 v_nearPoint;
out vec3 v_farPoint;

// grid position are in xy clipped space
vec3 gridPlane[6] = vec3[](
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, -1, 0), vec3(1, 1, 0)
);

vec3 unprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInverse = inverse(view);
    mat4 projectionInverse = inverse(projection);
    vec4 unprojectedPoint = viewInverse * projectionInverse * vec4(x, y, z, 1.0);

    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 point = gridPlane[gl_VertexID].xyz;

    // Unproject the vertices near and far from plane
    v_nearPoint = unprojectPoint(point.x, point.y, 0.0, u_view, u_projection).xyz;
    v_farPoint = unprojectPoint(point.x, point.y, 1.0, u_view, u_projection).xyz;

    // using directly the clipped coordinates
    gl_Position = vec4(point, 1.0);
}