#version 430 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;			//projMatric = lightSpaceMatrix
	mat4 modelViewMatrix;		//model = modelviewMatrix
	mat3 normalMatrix;			
} matrices;

mat4 view;						//view = view

// Layout of vertex attributes in VBO
layout (location = 0) in vec2 inPosition;	//aPos = inPosition
layout (location = 1) in vec2 inCoord;		//aTexCoords = inCoord
layout (location = 2) in vec3 inNormal;		//aNormal = inNormal

out vec2 vTexCoord;			//texcoord
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;


void main()
{
	FragPos = vec3(matrices.modelViewMatrix * vec4(inPosition, 0.0, 1.0);
    Normal = transpose(inverse(mat3(matrices.modelViewMatrix))) * inNormal;
	vTexCoord = inCoord;
	FragPosLightSpace = matrices.projMatrix * vec4(FragPos, 1.0);
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 0.0, 1.0);
	   	
}