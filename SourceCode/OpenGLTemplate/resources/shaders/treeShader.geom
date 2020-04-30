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

uniform float dist;
uniform float explodeFactor;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main()
{
    bool bExplodeObject;

    if (dist < 30.f)
        bExplodeObject = true;

    float localExplode = bExplodeObject? explodeFactor : 0.0;

    //Get triangle Normal
    vec3 e1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 e2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 n = normalize(cross(e1, e2));

    for (int i = 0; i < 3; i++) 
    {
        vec4 explodedPos = gl_in[i].gl_Position + vec4(localExplode * n, 0);
        gl_Position = matrices.projMatrix * matrices.modelViewMatrix * explodedPos; //gl_in[i].gl_Position;
        vTexCoord = vTexCoordPass[i];
        eyePosition = eyePositionPass[i];
        eyeNorm = eyeNormPass[i];
        EmitVertex();
    }
    EndPrimitive();

}