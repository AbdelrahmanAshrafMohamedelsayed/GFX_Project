uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;

void main() {
    vec4 color = texture(tex, tex_coord);
    color.rgb = mix(vec3(0.5), color.rgb, 0.5);
    color.rgb = pow(color.rgb, vec3(1.5));
    frag_color = color;
}