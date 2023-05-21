#version 330

uniform sampler2D tex;     // Uniform variable for the input texture
in vec2 tex_coord;         // Input variable for the texture coordinates of the fragment
out vec4 frag_color;       // Output variable for the final color of the fragment

void main()
{
    float offset = 0.007;  // Offset value used for the sample offsets and kernel

    float kernel[9];       // Array to store the kernel for the sharpening effect
    vec2 offsets[9];       // Array to store the offsets for the 9 samples surrounding the current pixel
    vec3 sampleTex[9];     // Array to store the sampled colors from the texture
    vec3 accumulated_color;              // Variable to accumulate the color values
    vec4 output_color = texture(tex, tex_coord);  // Original color from the texture

    // Define the offsets for the 9 samples surrounding the current pixel
    offsets = vec2[](
        vec2(-offset, offset),   // top-left
        vec2(0.0, offset),       // top-center
        vec2(offset, offset),    // top-right
        vec2(-offset, 0.0),      // center-left
        vec2(0.0, 0.0),          // center-center
        vec2(offset, 0.0),       // center-right
        vec2(-offset, -offset),  // bottom-left
        vec2(0.0, -offset),      // bottom-center
        vec2(offset, -offset)    // bottom-right
    );

    // Define the kernel for the sharpening effect
    kernel = float[](
        -1.0, -1.0, -1.0,
        -1.0, 9.0, -1.0,
        -1.0, -1.0, -1.0
    );

    accumulated_color = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        // Sample the texture using the offsets and store the sampled color
        sampleTex[i] = vec3(texture(tex, tex_coord + offsets[i]));
        accumulated_color += sampleTex[i] * kernel[i];
    }

    output_color = vec4(accumulated_color, 1.0);
    frag_color = output_color;
}
