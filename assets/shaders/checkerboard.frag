#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    // Get the pixel coordinates of the current fragment
    ivec2 coords = ivec2(gl_FragCoord.xy);

    // Determine which color to use based on the tile position
    // coords.x/size and coords.y/size will give us the tile coordinates
    // We want to add these together to get the tile index

    int color_idx = (int(coords.x/size) + int(coords.y/size)) % 2;
    vec3 color = colors[color_idx];
    // Output the resulting color as a vec4 with alpha set to 1.0 to indicate full opacity
    frag_color = vec4(color, 1.0);
}