#version 330

//Define max number of lights
#define MAX_LIGHTS 100
#define DirectionalLight 0
#define PointLight       1  
#define SpotLight        2

struct Light {
    int LightType;
    vec3 position;
    vec3 direction;
    vec3 diffuse;            // high diffuse => most affected by light color
    vec3 specular;           // high specular  => most affected by light color
    vec3 attenuation;        // Used by Point and Spotlight only
    vec2 cone_angles;        // Used by Spotlight only
};


uniform Light lights[MAX_LIGHTS];
uniform int light_count;

struct Sky {
    vec3 top, middle, bottom;
};

uniform Sky sky;

// Choose the light ambient based on the normal at the point
vec3 compute_sky_light(vec3 normal){
    vec3 extreme = normal.y > 0 ? sky.top : sky.bottom;
    return mix(sky.middle, extreme, normal.y * normal.y);
}

// Modify Material to include 5 Sampler2Ds
struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;             // Will get shineness from this
    sampler2D ambient_occlusion;
    sampler2D emissive;              // For objects that emit light
};

uniform Material material;

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

out vec4 frag_color;

float lambert(vec3 normal, vec3 world_to_light_direction) {
    return max(0.0, dot(normal, world_to_light_direction));
}

float phong(vec3 reflected, vec3 view, float shininess) {
    return pow(max(0.0, dot(reflected, view)), shininess);
}

void main() {
    vec3 normal = normalize(fs_in.normal);
    vec3 view = normalize(fs_in.view);
    
    vec3 ambient_light = compute_sky_light(normal);

    // Compute the 5 quantities by sampling the corresponding texture
    vec3 diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 specular = texture(material.specular, fs_in.tex_coord).rgb;

    // only the red component because the texture is greyscale
    float roughness = texture(material.roughness, fs_in.tex_coord).r;
    vec3 ambient = diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
    vec3 emissive = texture(material.emissive, fs_in.tex_coord).rgb;

    // Compute Shininess from Roughness
    // When roughness is 0 shineness is infinity and when its 1 shineness is 0 
    // but clamp the value to be away from 0 and infinite shineness
    // roughness texture from 0 , 1 can map to any possible shineness
    float shininess = 2.0 / pow(clamp(roughness, 0.001, 0.999), 4.0) - 2.0;
    
    vec3 world_to_light_dir;
    vec3 color = emissive + ambient_light * ambient;

    // Loop on the lights in the fragment shader and add the overall effect
    for(int light_idx = 0; light_idx < min(MAX_LIGHTS, light_count); light_idx++){
        Light light = lights[light_idx];
        float attenuation = 1.0;
        if(light.LightType == DirectionalLight){
            world_to_light_dir = -light.direction;
        } else {

            // Implement Attenuation for Point Light
            world_to_light_dir = light.position - fs_in.world;
            float d = length(world_to_light_dir);
            world_to_light_dir /= d;

            attenuation = 1.0 / dot(light.attenuation, vec3(d*d, d, 1.0));

            if(light.LightType == SpotLight){

                // Extend the implementation of Attenuation for Spotlight
                float angle = acos(dot(-world_to_light_dir,light.direction));
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

        vec3 computed_diffuse = light.diffuse * diffuse * lambert(normal, world_to_light_dir);

        //  Compute the reflection vector for specular and then the specular component
        vec3 reflected = reflect(-world_to_light_dir, normal);
        vec3 computed_specular = light.specular * specular * phong(reflected, view, shininess);

        // Add the effects from different lights
        color += (computed_diffuse + computed_specular) * attenuation;

    }
    
    frag_color = vec4(color, 1.0);

}