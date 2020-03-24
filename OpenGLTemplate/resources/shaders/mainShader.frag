#version 400 core

in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
out vec4 vOutputColour;		// The output colour

struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec3 direction;
	float exponent;
	float cutoff;
};

struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

uniform LightInfo light1; 
uniform LightInfo spotlight[4];
uniform MaterialInfo material1; 

in vec4 eyePosition;
in vec3 eyeNorm;

vec3 PhongModel(vec4 eyePosition, vec3 eyeNorm)
{
	vec3 s = normalize(vec3(light1.position - eyePosition));
	vec3 v = normalize(-eyePosition.xyz);
	vec3 r = reflect(-s, eyeNorm);
	vec3 n = eyeNorm;
	vec3 ambient = light1.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = light1.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0f);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	if (sDotN > 0.0f) 
		specular = light1.Ls * material1.Ms * pow(max(dot(r, v), 0.0f), material1.shininess + eps);

	return ambient + diffuse + specular;
}

vec3 BlinnPhongSpotlightModel(LightInfo light, vec4 eyePosition, vec3 eyeNorm)
{
	vec3 s = normalize(vec3(light.position - eyePosition));
	float angle = acos(dot(-s, light.direction));
	float cutoff = radians(clamp(light.cutoff, 0.0, 90.0));
	vec3 ambient = light.La * material1.Ma;
	if (angle < cutoff) {
		float spotFactor = pow(dot(-s, light.direction), light.exponent);
		vec3 v = normalize(-eyePosition.xyz);
		vec3 h = normalize(v + s);
		float sDotN = max(dot(s, eyeNorm), 0.0);
		vec3 diffuse = light.Ld * material1.Md * sDotN;
		vec3 specular = vec3(0.0);
		if (sDotN > 0.0)
			specular = light.Ls * material1.Ms * pow(max(dot(h, eyeNorm), 0.0),
			material1.shininess);
		return ambient + spotFactor * (diffuse + specular);
	} else
	return ambient;
}

uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform bool bUseTransparency; //A flag indicating if transparent texture mapping should be applied
uniform bool renderSkybox;
in vec3 worldPosition;


void main()
{
	vec3 vColour = PhongModel(eyePosition, normalize(eyeNorm));						//world light
	for (int i = 0; i < 4; i++) {
		vColour += BlinnPhongSpotlightModel(spotlight[i], eyePosition, normalize(eyeNorm));	//spotlight1
	}	

	if (renderSkybox) {
		vOutputColour = texture(CubeMapTex, worldPosition);

	} else {

		// Get the texel colour from the texture sampler
		vec4 vTexColour = texture(sampler0, vTexCoord);	

		if (bUseTexture)
			vOutputColour = vTexColour*vec4(vColour, 1.0f);	// Combine object colour and texture 
		else if (bUseTransparency)
			vOutputColour = vTexColour*vec4(vColour, 0.5f);	// Combine object colour and texture 
		else
			vOutputColour = vec4(vColour, 1.0f);	// Just use the colour instead
	}
	
	
}
