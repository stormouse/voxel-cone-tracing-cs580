#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main(){
    mat4 mvp = projectionTransform * viewTransform * M;
    gl_Position = mvp * vec4(position, 1);
}