#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 TexCoords;

layout (std140) uniform Matrices
{
    mat4 Projection;
    mat4 View;
    vec3 ViewPos;
};

uniform mat4 CubeMapView;

void main() 
{ 
	TexCoords = aPos;
    gl_Position = Projection * CubeMapView * vec4(aPos, 1.0);
}