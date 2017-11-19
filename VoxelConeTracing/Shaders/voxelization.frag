#version 430 core


layout(rgba8) uniform image3D voxelTexture;
uniform int voxelDimensions;


in vec3 worldPositionFrag;
in vec3 normalFrag;


vec3 scaleAndBias(vec3 p) { return 0.5f * p + vec3(0.5f); }


void main(){
    vec4 color = vec4(1.0f);
    vec3 voxel = scaleAndBias(worldPositionFrag);
    imageStore(voxelTexture, ivec3(voxelDimensions * voxel), color);
}
