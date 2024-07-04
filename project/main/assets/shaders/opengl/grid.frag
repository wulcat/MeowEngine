//
// Created by Akira Mujawar on 03/07/24.
//

in vec3 v_nearPoint;
in vec3 v_farPoint;

out vec4 v_FragColor;

void main() {
    float t = -v_nearPoint.y / (v_farPoint.y - v_nearPoint.y);

    v_FragColor = vec4(1.0, 0.0, 0.0, 1.0 * float(t > 0));
}