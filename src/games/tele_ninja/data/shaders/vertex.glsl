//#version 120

uniform mat4 projection;
uniform mat4 cam;

attribute vec3 vertex;
attribute vec2 tex_coord;

varying vec2 frag_tex_coord;

void main()
{
        gl_Position = cam * projection * vec4(vertex, 1.0);
        frag_tex_coord = tex_coord;
}

