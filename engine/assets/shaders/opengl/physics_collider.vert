//
// Created by Akira Mujawar on 03/07/24.
//

uniform mat4 u_view;
uniform mat4 u_projection;

layout(location = 0) in vec3 l_vertexPosition;
layout(location = 1) in mat4 u_transformation;

void main()
{
   gl_Position = u_transformation * vec4(l_vertexPosition, 1.0);
}