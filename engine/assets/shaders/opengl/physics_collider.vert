//
// Created by Akira Mujawar on 03/07/24.
//

uniform mat4 u_view;
uniform mat4 u_projection;

in vec3 a_vertexPosition;
in mat4 u_transformation;

void main()
{
   gl_Position = u_view * u_projection * u_transformation * vec4(a_vertexPosition, 1.0);
}