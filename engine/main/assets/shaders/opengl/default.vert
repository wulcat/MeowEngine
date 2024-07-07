//
// Created by Akira Mujawar on 03/07/24.
//
// model, view, projection - describles the transformation
// https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

uniform mat4 u_mvp;

in vec3 a_vertexPosition;
in vec2 a_textureCoord;

out vec2 v_textureCoord;

void main() {
    gl_Position = u_mvp * vec4(a_vertexPosition, 1.0);
    v_textureCoord = a_textureCoord;
}