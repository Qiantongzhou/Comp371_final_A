#version 330 core

const float PI = 3.1415926535897932384626433832795;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 light_direction;

uniform vec3 object_color;



uniform float light_cutoff_outer;
uniform float light_cutoff_inner;
uniform float light_near_plane;
uniform float light_far_plane;
uniform float shading_ambient_strength;  
uniform float shading_diffuse_strength;  
uniform float shading_specular_strength; 

uniform vec3 view_position;

uniform sampler2D shadow_map;

uniform sampler2D textureSampler;

in vec3 fragment_position;
in vec4 fragment_position_light_space;
in vec3 fragment_normal;
in vec2 vertexUV;
in vec3 vertexColor;

in vec4 gl_FragCoord;

out vec4 result;

vec3 ambient_color(vec3 light_color_arg) {
    return shading_ambient_strength * light_color_arg;
}

vec3 diffuse_color(vec3 light_color_arg, vec3 light_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    return shading_diffuse_strength * light_color_arg * max(dot(normalize(fragment_normal), light_direction), 0.0f);
}

vec3 specular_color(vec3 light_color_arg, vec3 light_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_light_direction = reflect(-light_direction, normalize(fragment_normal));
    return shading_specular_strength * light_color_arg * pow(max(dot(reflect_light_direction, view_direction), 0.0f),32);
}

float shadow_scalar() {
    // this function returns 1.0 when the surface receives light, and 0.0 when it is in a shadow
    // perform perspective divide
    vec3 normalized_device_coordinates = fragment_position_light_space.xyz / fragment_position_light_space.w;
    // transform to [0,1] range
    normalized_device_coordinates = normalized_device_coordinates * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragment_position_light_space as coords)
    float closest_depth = texture(shadow_map, normalized_device_coordinates.xy).r;
    // get depth of current fragment from light's perspective
    float current_depth = normalized_device_coordinates.z;
    // check whether current frag pos is in shadow
    float bias = 0.003;  // bias applied in depth map: see shadow_vertex.glsl
    return ((current_depth - bias) < closest_depth) ? 1.0 : 0.0;
}

float spotlight_scalar() {
    float theta = dot(normalize(fragment_position - light_position), light_direction);
    
    if(theta > light_cutoff_inner) {
        return 1.0;
    } else if(theta > light_cutoff_outer) {
        return (1.0 - cos(PI * (theta - light_cutoff_outer) / (light_cutoff_inner - light_cutoff_outer))) / 2.0;
    } else {
        return 0.0;
    }
}
float spotlight_scalar1() {
    float theta = dot(normalize(fragment_position - light_position), light_direction-vec3(0.4,-0.3,.4));
    
    if(theta > light_cutoff_inner) {
        return 1.0;
    } else if(theta > light_cutoff_outer) {
        return (1.0 - cos(PI * (theta - light_cutoff_outer) / (light_cutoff_inner - light_cutoff_outer))) / 2.0;
    } else {
        return 0.0;
    }
}

void main()
{
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    vec3 ambient1 = vec3(0.0f);
    vec3 diffuse1 = vec3(0.0f);
    vec3 specular1 = vec3(0.0f);

    vec3 textureColor;

    float scalar = shadow_scalar() * spotlight_scalar();
    float scalar1 = shadow_scalar() * spotlight_scalar1();
    ambient = ambient_color(light_color);
    diffuse = scalar * diffuse_color(light_color, light_position);
    specular = scalar * specular_color(light_color, light_position);

    ambient1 = ambient_color(light_color+vec3(1.,0.5,0.5));
    diffuse1 = scalar1 * diffuse_color(light_color+vec3(1.,0.0,0.0), (light_position+vec3(20.0,0.0,20.0)));
    specular1 = scalar1 * specular_color(light_color+vec3(1.,0.0,0.0), (light_position+vec3(20.0,0.0,20.0)));

    textureColor = texture( textureSampler, vertexUV ).rgb;
   
    vec3 color =(specular + diffuse + ambient) *object_color*(vec3(1.,1.0,1.0)+vertexColor)* textureColor;
    color+= (specular1 + diffuse1)*object_color*(vec3(1.,1.0,1.0)+vertexColor)* textureColor;
    
    result = vec4(color, 1.0f);
}