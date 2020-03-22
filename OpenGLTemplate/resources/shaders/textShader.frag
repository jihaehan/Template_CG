#version 400 core

in vec2 vTexCoord;
out vec4 vOutputColour;

uniform sampler2D sampler0;
uniform vec4 vColour;
uniform float t;

void main()
{
	vec4 vTexColour = texture(sampler0, vTexCoord);	// Get the texel colour from the image
	vOutputColour = vec4(vTexColour.r) * vColour;			// The texel colour is a grayscale value -- apply to RGBA and combine with vColor
	//vec4 rgbColour = vec4(sin(t)); //vec4(.8, cos(t/11)*0.5 + 0.5, .5, 1);
	//vOutputColour = vec4(vTexColour.r) * rgbColour;
}