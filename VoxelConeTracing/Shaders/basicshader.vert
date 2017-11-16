#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main(){
    mat4 mvp = projectionTransform * viewTransform * modelTransform;
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1);
}