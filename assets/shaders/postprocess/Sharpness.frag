#version 330

// The texture holding the pixels
uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;

void main(){
    float offset = 0.007;

    float kernel[9];
    vec2 offsets[9];
    vec3 sampleTex[9];
    vec3 col;
    vec4 outColour = texture(tex, tex_coord);

    // Define the offsets for the 9 samples surrounding the current pixel
    offsets = vec2[](
        vec2(-offset, offset),   // top-left
        vec2(0.0f, offset),      // top-center
        vec2(offset, offset),    // top-right
        vec2(-offset, 0.0f),     // center-left
        vec2(0.0f, 0.0f),        // center-center
        vec2(offset, 0.0f),      // center-right
        vec2(-offset, -offset),  // bottom-left
        vec2(0.0f, -offset),     // bottom-center
        vec2(offset, -offset)    // bottom-right
    );

    // Define the kernel for the sharpening effect
    kernel = float[](
        -1.0, -1.0, -1.0,
        -1.0, 9.0, -1.0,
        -1.0, -1.0, -1.0
    );

    col = vec3(0.0);
    for(int i = 0; i < 9; i++) {
        // Sample the texture using the offsets and store the sampled color
        sampleTex[i] = vec3(texture(tex, tex_coord + offsets[i]));
        col += sampleTex[i] * kernel[i];
    }

    outColour = vec4(col, 1.0);
    frag_color = outColour;
}
