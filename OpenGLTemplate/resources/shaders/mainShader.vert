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

out vec2 vTexCoord;	// Texture coordinate
out vec3 worldPosition;	// used for skybox
out vec4 eyePosition; 
out vec3 eyeNorm;

void main()
{	

	// Save the world position for rendering the skybox
	worldPosition = inPosition;

	// Transform the vertex spatial position using 
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	
	// Get the vertex normal and vertex position in eye coordinates
	eyeNorm = normalize(matrices.normalMatrix * inNormal);
	eyePosition = matrices.modelViewMatrix * vec4(inPosition, 1.0f);

	// Pass through the texture coordinate
	vTexCoord = inCoord;
} 
	