#version 330

precision highp float; 

struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

uniform lightStruct spotLight;
uniform	vec4 spotLightPosition;				// Position in eye coords.
uniform vec3 spotlightDirection;			// Normalized direction of the spotlight
uniform int angle1;							// Angle of outer cone
uniform	int angle2;							// Angle of inner cone
in vec4 Vposition;							// Vertex position


uniform float attConst;
uniform float attLinear;
uniform float attQuadratic;

in  vec4 ex_Color;
out vec4 out_Color;
uniform sampler2D textureUnit0;


void main(void)
{
	float DEG_TO_RADIANS = 0.017453293;

	float ang1=  angle1 * DEG_TO_RADIANS;
	float ang2 = angle2 * DEG_TO_RADIANS;

	//Spotlight calculations
	vec4 totalLight;

	//Finds the vector from the spotlight to the vertex
	vec3 L =   spotLightPosition.xyz - Vposition.xyz ;
	float distToLight = length(L);									//this could be used for attenuation
	L = normalize(L);

	//The angle between spotlight direction and vertex
	float cosDir = dot(L, -spotlightDirection);

	//Compare vector to frag from light to the cones angles
	float spotEffect = smoothstep(cos(ang2) , cos(ang1) ,cosDir);

	//Add the spotlight light values and times by spoteffect. 
	//Spoteffect will be zero if the current fragment is not in the spotlights light cone
	vec4 lightSpot = vec4(spotEffect) * (spotLight.diffuse + spotLight.specular);

	totalLight = vec4(lightSpot);
	

	//Ditch low values of transparency
	if(texture(textureUnit0, gl_PointCoord).a < 0.5) discard;

	out_Color = (ex_Color + totalLight) * vec4(ex_Color.a) ;
	
	 
}