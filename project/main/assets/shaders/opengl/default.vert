// model, view, projection - describles the transformation
// https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
uniform mat4 u_mvp;

attribute vec3 a_vertexPosition;
attribute vec2 a_textureCoord;

varying vec2 v_textureCoord;

void main() {
    gl_Position = u_mvp * vec4(a_vertexPosition, 1.0);
    v_textureCoord = a_textureCoord;
    //fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
}