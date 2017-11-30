#version 330

precision highp float; // needed only for version 1.30

in  vec4 ex_Color;
out vec4 out_Color;
uniform sampler2D textureUnit0;


void main(void)
{

  //if (texture(textureUnit0, gl_PointCoord).r < 0.5) discard;
  if(texture(textureUnit0, gl_PointCoord).a < 0.5) discard;

	out_Color = ex_Color * vec4(ex_Color.a) ;// * texture(textureUnit0, gl_PointCoord);
	//out_Color = texture(textureUnit0, gl_PointCoord);
	 
}