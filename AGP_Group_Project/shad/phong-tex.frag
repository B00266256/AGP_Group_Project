// Phong fragment shader phong-tex.frag matched with phong-tex.vert
#version 330

// Some drivers require the following
precision highp float;

struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct materialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};


uniform lightStruct light;
uniform lightStruct spotLight;

uniform	vec4 spotLightPosition;				// Position in eye coords.
uniform vec3 spotlightDirection;			// Normalized direction of the spotlight
uniform int angle1;
uniform	int angle2;



uniform materialStruct material;
uniform sampler2D textureUnit0;


uniform float attConst;
uniform float attLinear;
uniform float attQuadratic;

in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L;
in vec2 ex_TexCoord;
in float ex_D;
layout(location = 0) out vec4 out_Color;

in vec4 Vposition;
 
void main(void) {

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

	totalLight = vec4(lightSpot) * (material.ambient + material.diffuse + material.specular);
	totalLight = totalLight * 1.0f/(attConst + attLinear * distToLight + attQuadratic * distToLight*distToLight);
	
    
	// Ambient intensity
	vec4 ambientI = light.ambient * material.ambient;

	// Diffuse intensity
	vec4 diffuseI = light.diffuse * material.diffuse;
	diffuseI = diffuseI * max(dot(normalize(ex_N),normalize(ex_L)),0);

	// Specular intensity
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-ex_L),normalize(ex_N)));

	vec4 specularI = light.specular * material.specular;
	specularI = specularI * pow(max(dot(R,ex_V),0), material.shininess);

	//
		float attenuation=1.0f/(attConst + attLinear * ex_D + attQuadratic * ex_D*ex_D);

	vec4 tmp_Color = (diffuseI + specularI);

	//Attenuation does not affect transparency
	vec4 litColour = vec4(tmp_Color.rgb *attenuation, tmp_Color.a);
	vec4 amb=min(ambientI,vec4(1.0f));
	//

	// Fragment colour
	//out_Color = (ambientI + diffuseI + specularI) * texture(textureUnit0, ex_TexCoord);
	out_Color=min( ((litColour + totalLight ) + amb )*texture(textureUnit0, ex_TexCoord),vec4(1.0f) );
}