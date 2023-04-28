#version 330 core

in Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} fs_in;

out vec4 frag_color;

uniform sampler2D tex;

void main(){
    //TODO: (Req 5) Change the following line to read the fragment color
    // set the fragment color from the texture instead of the color passed from the vertex shader.
    frag_color = texture(tex, fs_in.tex_coord);
}