// phong-tex.vert
// Vertex shader for use with Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2.frag
#version 330

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 lightPosition;
uniform vec4 dirLightPosition;
uniform vec4 spotLightPosition;

//uniform mat3 normalmatrix;

in  vec3 in_Position;
in  vec3 in_Normal;
out vec3 ex_N;
out vec3 ex_V;
out vec3 ex_L;
out vec3 ex_DL;

in vec2 in_TexCoord;
out vec2 ex_TexCoord;
out float ex_D;


out vec4 Vposition;


// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader
void main(void) {

	// vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);
	ex_D = distance(vertexPosition,lightPosition);
	// Find V - in eye coordinates, eye is at (0,0,0)
	ex_V = normalize(-vertexPosition).xyz;

	// surface normal in eye coordinates
	// taking the rotation part of the modelview matrix to generate the normal matrix
	// (if scaling is includes, should use transpose inverse modelview matrix!)
	// this is somewhat wasteful in compute time and should really be part of the cpu program,
	// giving an additional uniform input
	mat3 normalmatrix = transpose(inverse(mat3(modelview)));
	ex_N = normalize(normalmatrix * in_Normal);

	// L - to light source from vertex
	ex_L = normalize(lightPosition.xyz - vertexPosition.xyz);
	ex_DL = vec3(normalize(dirLightPosition));
	ex_TexCoord = in_TexCoord;

	//spotlight
	Vposition = vertexPosition;
	

    gl_Position =  * vertexPosition;

}