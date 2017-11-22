#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 geomPosition_worldspace[];
in vec3 geomNormal[];

out vec3 fragPosition_worldspace;
out vec3 fragNormal;

void main(){
	const vec3 p1 = geomPosition_worldspace[1] - geomPosition_worldspace[0];
	const vec3 p2 = geomPosition_worldspace[2] - geomPosition_worldspace[0];
	const vec3 p = abs(cross(p1, p2)); 
	for(uint i = 0; i < 3; ++i){
		fragPosition_worldspace = geomPosition_worldspace[i];
		fragNormal = geomNormal[i];
		if(p.z > p.x && p.z > p.y){
			gl_Position = vec4(fragPosition_worldspace.x, fragPosition_worldspace.y, 0, 1);
		} else if (p.x > p.y && p.x > p.z){
			gl_Position = vec4(fragPosition_worldspace.y, fragPosition_worldspace.z, 0, 1);
		} else {
			gl_Position = vec4(fragPosition_worldspace.x, fragPosition_worldspace.z, 0, 1);
		}
		EmitVertex();
	}
    EndPrimitive();
}