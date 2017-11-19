#version 430 core

out vec3 color;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 emissivity;
uniform vec3 transparency;
uniform vec3 refractiveIndex;
uniform vec3 specularReflectivity;
uniform vec3 diffuseReflectivity;
uniform vec3 specularDiffusion;


void main(){
    color = diffuseColor;
}