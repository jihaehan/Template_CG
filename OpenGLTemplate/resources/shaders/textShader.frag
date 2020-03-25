#version 400 core

in vec2 vTexCoord;
out vec4 vOutputColour;

uniform sampler2D sampler0;
uniform vec4 vColour;
uniform float t;
uniform bool bText;

void main()
{
	vec4 rgbColour = vec4(0.5 + 0.4*sin(t), 0.5 + 0.4* cos(t), 0.5 + 0.1*sin(t), 1); 

	if (bText)
	{
		vec4 vTexColour = texture(sampler0, vTexCoord);	// Get the texel colour from the image
		//vOutputColour = vec4(vTexColour.r) * vColour;			// The texel colour is a grayscale value -- apply to RGBA and combine with vColor
		vOutputColour = vec4(vTexColour.r) * rgbColour;
	}
	else 
	{
		vec4 vTexColour = texture(sampler0, vTexCoord);
		vOutputColour = vTexColour * rgbColour;
	}
}