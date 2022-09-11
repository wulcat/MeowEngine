// model, view, projection - describles the transformation
// https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
uniform mat4 mvp;

attribute vec3 vertexPosition;

varying vec4 fragmentColor;

void main() {
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
}