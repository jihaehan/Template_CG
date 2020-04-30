#version 400 core

uniform sampler2D sampler0;

in vec3 vColour;			// Note: colour is smoothly interpolated (default)
in vec2 vTexCoord;

out vec4 vOutputColour;


void main()
{	
	vec4 vTexColour = texture(sampler0, vTexCoord);
	vOutputColour = vec4(vColour, 1.0f) * vTexColour;
	
}
