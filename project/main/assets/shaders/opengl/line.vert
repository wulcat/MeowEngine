//
// Created by Akira Mujawar on 03/07/24.
//

uniform mat4 u_mvp;
uniform vec3 u_worldPosition;
uniform vec3 u_cameraPosition;

attribute vec3 a_vertexPosition;

// calculate so frag shader can do a fade color by distance
varying float o_vertexDistance;

void main()
{
   gl_Position = u_mvp * vec4(a_vertexPosition, 1.0);
   o_vertexDistance = length(u_cameraPosition - u_worldPosition);
}