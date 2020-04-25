#version 400

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;

in vec2 vTexCoordPass[];
in vec4 eyePositionPass[]; 
in vec3 eyeNormPass[];

out vec2 vTexCoord;
out vec4 eyePosition; 
out vec3 eyeNorm;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main()
{
    // Pass to fragment shader
    for (int i = 0; i < 3; i++) 
    {
        gl_Position = matrices.projMatrix * matrices.modelViewMatrix * gl_in[i].gl_Position;
        vTexCoord = vTexCoordPass[i];
        eyePosition = eyePositionPass[i];
        eyeNorm = eyeNormPass[i];
        EmitVertex();
    }
    EndPrimitive();

}