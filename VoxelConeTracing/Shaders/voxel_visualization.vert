#version 420 core

uniform mat4 V;

in vec3 position;
out vec2 textureCoordinateFrag; 

// Scales and bias a given vector (i.e. from [-1, 1] to [0, 1]).
vec2 scaleAndBias(vec2 p) { return 0.5f * p + vec2(0.5f); }

void main(){
	textureCoordinateFrag = scaleAndBias(position.xy);
	gl_Position = vec4(position, 1);
}