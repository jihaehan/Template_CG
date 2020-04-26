#version 400 core

uniform int levels;
uniform sampler2D sampler0;

in vec3 vColour;			// Note: colour is smoothly interpolated (default)
in vec2 vTexCoord;

out vec4 vOutputColour;
uniform float changeColour;

void main()
{
	vec3 quantisedColour = floor(vColour * levels) / levels;

	vec4 vTexColour = texture(sampler0, vTexCoord);

	vOutputColour = vec4(vec3(quantisedColour.r + changeColour, quantisedColour.g, quantisedColour.b - changeColour/2), 0.6f) ;//* vTexColour; 
		
}
