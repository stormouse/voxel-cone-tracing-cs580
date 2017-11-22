#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 geomPosition_worldspace;
out vec3 geomNormal;

void main(){
	geomPosition_worldspace = vec3(M * vec4(position, 1));
	geomNormal = normalize(mat3(transpose(inverse(M))) * normal);
	gl_Position = P * V * vec4(geomPosition_worldspace, 1);
}