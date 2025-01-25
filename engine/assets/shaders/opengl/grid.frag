//
// Created by Akira Mujawar on 03/07/24.
//
// Reference: https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/

uniform mat4 u_view;
uniform mat4 u_projection;

in vec3 v_nearPoint;
in vec3 v_farPoint;

layout(location = 0) out vec4 v_fragColor;

vec4 grid(vec3 fragPos3D, float scale, bool boolAxis) {
    vec2 coord = fragPos3D.xz * scale; // use the scale variable to set the distance between the lines
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float alpha = 1.0 - min(line, 1.0);
    vec4 color = vec4(0.3, 0.3, 0.3, alpha);

    float minimumz = min(derivative.y , 1.0);
    float minimumx = min(derivative.x , 1.0);

    // z axis
    if(fragPos3D.x > -0.5 * minimumx && fragPos3D.x < 0.5 * minimumx) {
        color.z = 1.0;
    }
    // x axis
    if(fragPos3D.z > -0.5 * minimumz && fragPos3D.z < 0.5 * minimumz) {
        color.x = 1.0;
    }

    return color;
}

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
        float fading = max(0.0, (0.4 - linearDepth));

        v_fragColor = (grid(fragPos3D, 1.5, true) + grid(fragPos3D, 0.25, true)) ;//* float(t > 0);
        v_fragColor.a *= fading;
    }
    else {
        // just little blue for adding light to scene
        float linearDepth = computeLinearDepth(fragPos3D, 60.0);
        float fading = max(0.0, (0.0 - linearDepth));

        //v_fragColor = vec4(147.0/255.0, 209.0/255.0, 255.0/255.0 , 1.0);
        v_fragColor.a *= 1.0 - fading;
    }
}

