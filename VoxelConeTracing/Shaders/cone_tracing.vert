#version 430 core

in vec3 position;
in vec3 normal;

out vec3 worldPosFrag;
out vec3 normalFrag;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main(){
    worldPosFrag = vec3(M*vec4(position, 1));
    gl_Position = P*V*vec4(worldPosFrag, 1);
    normalFrag = normalize(mat3(transpose(inverse(M))) * normal);
}
