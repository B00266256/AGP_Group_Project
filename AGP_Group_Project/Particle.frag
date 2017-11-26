#version 330

precision highp float; // needed only for version 1.30


struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

uniform lightStruct spotLight;

uniform	vec4 spotLightPosition;				// Position in eye coords.
uniform vec3 spotlightDirection;			// Normalized direction of the spotlight
uniform int angle1;
uniform	int angle2;

uniform float attConst;
uniform float attLinear;
uniform float attQuadratic;

in  vec4 ex_Color;
out vec4 out_Color;
uniform sampler2D textureUnit0;

in vec4 Vposition;

void main(void)
{

	float ang1=  radians(angle1);
	float ang2 = radians(angle2);

	//Spotlight calculations
	vec4 totalLight;

	//Finds the vector from the spotlight to the vertex
	vec3 L =  Vposition.xyz;		//spotLightPosition.xyz -
	float distToLight = length(L);									//this could be used for attenuation
	L = normalize(L);

	//The angle between spotlight direction and vertex
	float cosDir = dot(L, -spotlightDirection);

	//Compare vector to frag from light to the cones angles
	float spotEffect = smoothstep(cos(ang2) , cos(ang1) ,cosDir);

	//spotEffect *= max(dot(normalize(ex_N),normalize(L)),0);

	//Add the spotlight light values and times by spoteffect. 
	//Spoteffect will be zero if the current fragment is no in the spotlights light cone
	vec4 lightSpot = vec4(spotEffect) * (spotLight.diffuse + spotLight.specular) ;

	//lightSpot = lightSpot * max(dot(normalize(ex_N),normalize(L)),0);

	totalLight = vec4(lightSpot);
	totalLight = totalLight * 1.0f/(attConst + attLinear * distToLight + attQuadratic * distToLight*distToLight);
	

	//If its black then it's not part of the texture we want, discard it
  if (texture(textureUnit0, gl_PointCoord) == vec4(0,0,0,1)) discard;
 

	out_Color = (ex_Color ) * vec4(ex_Color.a);
	out_Color += totalLight;
	//out_Color = texture(textureUnit0, gl_PointCoord);
	 
}