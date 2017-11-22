#version 430 core

in vec3 worldPositionFrag;
in vec3 normalFrag;

struct PointLight{
    vec3 position;
    vec3 color;
    float intensity;
};

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float diffuseReflectivity;
    float specularReflectivity;
    float emissivity;
    float transparency;
};


uniform int voxelDimensions;
uniform Material material;
uniform PointLight pointLights[1];
uniform int numOfLights;
layout(rgba8) uniform image3D voxelTexture;


float attenuate(float dist){
    return 1.0 / (1.0 + dist * dist);
}
vec3 scaleAndBias(vec3 p) { return 0.5f * p + vec3(0.5f); }

vec3 calculatePointLight(const PointLight light){
    vec3 direction = normalize(light.position - worldPositionFrag);
    float distanceToLight = distance(light.position, worldPositionFrag);
    float attenuation = attenuate(distanceToLight);
    float diff = clamp(dot(direction, normalize(normalFrag)), 0.0, 1.0);
    return light.intensity * attenuation * light.color * diff;
}


void main(){
    vec4 color = vec4(1.0f);
    int n_lights = min(numOfLights, 1);

    for(int i=0;i<n_lights;i++){
        color += calculatePointLight(pointLights[i]);
    }

    vec3 spec = material.specularReflectivity * material.specularColor;
    vec3 diff = material.diffuseReflectivity * material.diffuseColor;
    color = (diff + spec) * color + clamp(material.emissivity, 0, 1) * material.diffuseColor;

    vec3 voxel = scaleAndBias(worldPositionFrag);
    float alpha = pow(1 - material.transparency, 4);
    vec4 res = alpha * vec4(vec3(color), 1);
    imageStore(voxelTexture, ivec3(voxelDimensions * voxel), res);
}
