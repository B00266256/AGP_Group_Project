#version 330

precision highp float; // needed only for version 1.30

in  vec3 ex_Color;
out vec3 out_Color;
uniform sampler2D textureUnit0;


void main(void)
{

  //if (ex_Color.a < 0.2) discard;

	out_Color = ex_Color;   // *  texture(textureUnit0, gl_PointCoord);
	 
}
