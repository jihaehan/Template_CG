#version 400 core

layout (location = 0) in vec3 aPosition;

uniform struct Matrices
{
	mat4 modelViewMatrix;
	mat4 lightSpaceMatrix;
} matrices;


void main()
{
	gl_Position = matrices.lightSpaceMatrix * matrices.modelViewMatrix * vec4(aPosition, 1.0);
}  

