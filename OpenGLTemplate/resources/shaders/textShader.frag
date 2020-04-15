#version 400 core

in vec2 vTexCoord;
out vec4 vOutputColour;

uniform sampler2D sampler0;
uniform vec4 vColour;
uniform float t;
uniform bool bText;
uniform bool bRGB;
uniform int switchColour; 

void main()
{
	vec4 rgbColour = vec4(0.5 + 0.4*sin(t), 0.5 + 0.4* cos(t), 0.5 + 0.1*sin(t), 1); 
	vec4 vTexColour = texture(sampler0, vTexCoord);

	if (bText)
	{
		//vOutputColour = vec4(vTexColour.r) * vColour;			// The texel colour is a grayscale value -- apply to RGBA and combine with vColor
		vOutputColour = vec4(vTexColour.r) * rgbColour;
	}
	else if (bRGB) 
	{
		vOutputColour = vTexColour * rgbColour;
	}
	else
	{
		if (switchColour == 0)
			vOutputColour = vTexColour;
		else if (switchColour == 1)
			vOutputColour = vec4(vec3(vTexColour.r), 1.0);
		else if (switchColour == 2) 
			vOutputColour = vec4(vTexColour.g, vTexColour.b, vTexColour.r, 1);
		else if (switchColour == 3)
			vOutputColour = vec4(vTexColour.r*vTexColour.r, vTexColour.g * vTexColour.g, vTexColour.b *vTexColour.b, 1);
		else if (switchColour == 4)
			vOutputColour = vTexColour * rgbColour;
		else if (switchColour == 5)
			vOutputColour = vec4(1 - vTexColour.r, 1 - vTexColour.g, 1 - vTexColour.b, 1);
	}	 
}
