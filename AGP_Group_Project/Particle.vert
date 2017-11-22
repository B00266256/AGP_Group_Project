// Vertex Shader – file "minimal.vert"

#version 330

in  vec3 in_Position;
in  vec3 in_Color;
out vec3 ex_Color;

uniform mat4 modelview;
uniform mat4 projection;

void main(void)
{
	ex_Color = in_Color;

	vec4 vertexPosition = modelview * vec4(in_Position,1.0);
	gl_Position = projection * vertexPosition;

   
	
}