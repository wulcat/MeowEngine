//
// Created by Akira Mujawar on 03/07/24.
//

uniform vec3 u_color;
uniform float u_maxDistance;

varying float o_vertexDistance;

void main() {
   float t = clamp(o_vertexDistance / u_maxDistance, 0.0, 1.0);
   gl_FragColor = mix(vec4(u_color, 1.0), vec4(u_color, 0.0), t);
}