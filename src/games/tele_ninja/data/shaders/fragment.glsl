#version 120
//precision highp float;

uniform sampler2D sprite_texture;

varying vec2 frag_tex_coord;

void main()
{
        gl_FragColor = texture2D(sprite_texture, frag_tex_coord);
}
