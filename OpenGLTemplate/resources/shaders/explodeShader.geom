#version 400

in vec3 vColourPass[];
in vec2 vTexCoordPass[];

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;


out vec3 vColour;
out vec2 vTexCoord;
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform bool bExplodeObject;
uniform float explodeFactor;

void main()
{
    float localExplode = bExplodeObject? explodeFactor: 0.0;

    //Get triangle Normal
    vec3 e1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 e2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 n = normalize(cross(e1, e2));
    for (int i = 0; i < 3; i++) 
    {
        vec4 explodedPos = gl_in[i].gl_Position + localExplode * vec4(n, 0);
        gl_Position = matrices.projMatrix * matrices.modelViewMatrix * explodedPos;
        vColour = vColourPass[i];
        vTexCoord = vTexCoordPass[i];
        EmitVertex();
    }
    EndPrimitive();

}