#version 330 core

out vec4 frag_color;  // Output variable for the final color of the fragment
in vec2 tex_coord;    // Input variable for the texture coordinates of the fragment
uniform sampler2D tex;       // Uniform variable for the input texture

uniform float exposure = 3;  // Uniform variable for the exposure value (default: 3)

void main()
{
    float gamma = 1;  // Parameter for gamma correction (default: 1)

    // Take the color from the texture using the provided texture coordinates
    vec3 hdrColor = texture(tex, tex_coord).rgb;

    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));

    // Assign the final color to the output variable
    frag_color = vec4(mapped, 1.0);
}
