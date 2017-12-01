// phong-tex.vert
// Vertex shader for use with a Phong or other reflection model fragment shader
// Calculates and passes on V, L, N vectors for use in fragment shader, phong2.frag
// Passes vertex position for use in calculating spotlight
#version 330

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 lightPosition;
uniform vec4 spotLightPosition;

in  vec3 in_Position;
in  vec3 in_Normal;
in vec2 in_TexCoord;
out vec3 ex_N;
out vec3 ex_V;
out vec3 ex_L;
out float ex_D;
out vec2 ex_TexCoord;
out vec4 Vposition;


// multiply each vertex position by the MVP matrix
// and find V, L, N vectors for the fragment shader

void main(void) {

	// vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);

	// Find V - in eye coordinates, eye is at (0,0,0)
	ex_V = normalize(-vertexPosition).xyz;

	// surface normal in eye coordinates
	mat3 normalmatrix = transpose(inverse(mat3(modelview)));
	ex_N = normalize(normalmatrix * in_Normal);

	// L - to light source from vertex
	ex_L = normalize(lightPosition.xyz - vertexPosition.xyz);

	//Pass on texture coordinate
	ex_TexCoord = in_TexCoord;

	//Global position
    gl_Position = projection * vertexPosition;

	//Positional light distance
	ex_D = distance(vertexPosition,lightPosition);

	
	//For spotlight use
	Vposition = vertexPosition;

}