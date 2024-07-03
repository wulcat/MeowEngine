//
// Created by Akira Mujawar on 03/07/24.
//
// for kickstart - https://marcelbraghetto.github.io/a-simple-triangle/2019/04/25/part-10/

uniform sampler2D u_sampler;

varying vec2 v_textureCoord;

void main() {
    gl_FragColor = texture2D(u_sampler, v_textureCoord);
}

