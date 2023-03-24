#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    // frag_color = vec4(colors[0], 1.0);
    ivec2 coords = ivec2(gl_FragCoord.xy);

    // Determine which color to use based on the tile position
    int color_idx = (int(coords.x/size) + int(coords.y/size)) % 2;
    vec3 color = colors[color_idx];

    frag_color = vec4(color, 1.0);
}