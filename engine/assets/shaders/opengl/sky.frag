//
// Created by Akira Mujawar on 03/07/24.
//

uniform mat4 u_view;
uniform mat4 u_projection;

in vec3 v_nearPoint;
in vec3 v_farPoint;

layout(location = 0) out vec4 v_fragColor;

float computeDepth(vec3 pos) {
    vec4 clip_space_position = u_projection * u_view * vec4 (pos.xyz, 1.0);
    return 0.5 + 0.5 * (clip_space_position.z / clip_space_position.w);
}

float computeLinearDepth(vec3 pos, float far) {
    float near = 0.01;
    vec4 clip_space_pos = u_projection * u_view * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
    return linearDepth / far; // normalize
}

void main() {
    float t = -v_nearPoint.y / (v_farPoint.y - v_nearPoint.y);
    vec3 fragPos3D = v_nearPoint + t * (v_farPoint - v_nearPoint);
    gl_FragDepth = computeDepth(fragPos3D);

    if(t > 0.0) {
        float linearDepth = computeLinearDepth(fragPos3D, 45.0);
        float fading = max(0.0, (1.0 - linearDepth));

        v_fragColor = vec4(18.0/255.0, 18.0/255.0, 18.0/255.0 , 1.0);
        v_fragColor.a *= 1.0 - fading;
    }
    else {
        v_fragColor = vec4(18.0/255.0, 18.0/255.0, 18.0/255.0 , 1.0);
    }
}

