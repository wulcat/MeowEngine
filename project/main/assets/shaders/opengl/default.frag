//
// Created by Akira Mujawar on 03/07/24.
//
// for kickstart - https://marcelbraghetto.github.io/a-simple-triangle/2019/04/25/part-10/

uniform sampler2D u_sampler;

in vec2 v_textureCoord;

out vec4 v_fragColor;

void main() {
    v_fragColor = texture(u_sampler, v_textureCoord);
}

