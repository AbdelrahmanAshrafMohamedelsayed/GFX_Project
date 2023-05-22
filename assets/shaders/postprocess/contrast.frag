#version 330
uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;

void main() {
    vec4 color = texture(tex, tex_coord);
    // interpolate between gray color and original color
    color.rgb = mix(vec3(0.5), color.rgb, 0.5);
    // then raise color to power 1.5 to enhence contrast
    color.rgb = pow(color.rgb, vec3(1.5));
    frag_color = color;
}