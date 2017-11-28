#version 330

in  vec3 in_Position;
in  vec4 in_Color;
out vec4 ex_Color;

uniform mat4 modelview;
uniform mat4 projection;

void main(void)
{
	ex_Color = in_Color;

	vec4 vertexPosition = modelview * vec4(in_Position,1.0);


	//Calculate diatcne from point to camera and
	//scale point size so size is consistent at varying distances

	float distance = distance(vec3(vertexPosition), vec3(0,0,0));
	gl_PointSize = 100/ distance;


	gl_Position = projection * vertexPosition;

 
}