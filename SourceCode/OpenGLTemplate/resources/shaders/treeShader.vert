#version 400 core

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out vec2 vTexCoordPass;	// Texture coordinate
out vec4 eyePositionPass; 
out vec3 eyeNormPass;

void main()
{	
	// Instanced rendering
	float x = gl_InstanceID % 6;
	float z = gl_InstanceID / 6 ;
	vec3 shift = 10 * vec3(x, 0, z);

	// Transform the vertex spatial position using 
	gl_Position = vec4(inPosition + shift, 1.0f);

	// Get the vertex normal and vertex position in eye coordinates
	eyeNormPass = normalize(matrices.normalMatrix * inNormal);
	eyePositionPass = matrices.modelViewMatrix * vec4(inPosition, 1.0f);

	//  through the texture coordinate
	vTexCoordPass = inCoord;
} 
	