#version 400 core

uniform int levels;
uniform sampler2D sampler0;

in vec3 vColour;			// Note: colour is smoothly interpolated (default)
in vec2 vTexCoord;

out vec4 vOutputColour;


void main()
{	
	//vOutputColour = vec4(vColour, 1.0);
	vec3 quantisedColour = floor(vColour * levels) / levels;
	vec4 vTexColour = texture(sampler0, vTexCoord);

	vOutputColour = vec4(quantisedColour, 0.6f);// * vTexColour;
	
}
