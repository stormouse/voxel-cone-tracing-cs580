#version 430 core

in vec3 fragPosition_worldspace;
in vec3 fragNormal;

uniform vec3 	diffuseColor;
uniform float 	diffuseReflectivity;
uniform float 	transparency;

uniform vec3 	lightPosition;
uniform vec3 	lightColor;
uniform float 	lightIntensity;

uniform int		voxelDimension;

layout(rgba8) uniform image3D texture3D;

void main(){
	// calculate point light color
	float dist = distance(lightPosition, fragPosition_worldspace);
	float attenuate = 1.2 / (1.0 + dist * dist);
	vec3 direction = normalize(lightPosition - fragPosition_worldspace);
	float affine = clamp(dot(direction, fragNormal), 0, 1);
	vec3 color = lightColor * lightIntensity * affine * attenuate;
		 color = color * diffuseColor * diffuseReflectivity;
	vec3 voxel = 0.5f * fragPosition_worldspace + vec3(0.5f);
	float alpha = pow(1 - transparency, 4);
	vec4 res = alpha * vec4(color, 1);
	imageStore(texture3D, ivec3(voxelDimension * voxel), res); 
}