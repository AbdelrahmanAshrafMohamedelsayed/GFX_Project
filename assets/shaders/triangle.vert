#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

out Varyings {
    vec3 color;
} vs_out;

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).
// uniform vec2 translation;
// uniform vec2 scale;
uniform vec2 translation = vec2(0.0, 0.0);
uniform vec2 scale = vec2(1.0, 1.0);


//TODO: (Req 1) Finish this shader

void main(){
    // first, we need to define the positions and colors of the vertices
   vec3 positions[3] = vec3[3] (
        vec3(-0.5, -0.5, 0.0),
        vec3( 0.5, -0.5, 0.0),
        vec3( 0.0,  0.5, 0.0)
    );
    vec3 colors[3] = vec3[3] (
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
    );
    // then, we need to send the color to the fragment shader
    vs_out.color = colors[gl_VertexID];
    // finally, we need to transform the position of the vertices
    // we can do this by multiplying the position by the scale and adding the translation
    // we can use the gl_VertexID to index into the positions array
    vec2 transformedPosition = scale * positions[gl_VertexID].xy + translation;
    gl_Position = vec4(transformedPosition, 0.0, 1.0);
}