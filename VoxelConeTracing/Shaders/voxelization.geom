#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 worldPositionGeom[];
in vec3 normalGeom[];

out vec3 worldPositionFrag;
out vec3 normalFrag;


void main(){
    vec3 p1 = worldPositionGeom[1] - worldPositionGeom[0];
    vec3 p2 = worldPositionGeom[2] - worldPositionGeom[0];
    vec3 n= abs(cross(p1, p2));
    
    for(uint i = 0; i < 3; i++){
        worldPositionFrag = worldPositionGeom[i];
        normalFrag = normalGeom[i];

        if(n.z > n.x && n.z > n.y){
            gl_Position = vec4(worldPositionFrag.x, worldPositionFrag.y, 0, 1);
        } 
        else if (n.x > n.y && n.x > n.z){
            gl_Position = vec4(worldPositionFrag.y, worldPositionFrag.z, 0, 1);
        }
        else {
            gl_Position = vec4(worldPositionFrag.x, worldPositionFrag.z, 0, 1);
        }
        EmitVertex();
    }
    EndPrimitive();
}