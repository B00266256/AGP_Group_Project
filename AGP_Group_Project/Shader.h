#ifndef SHADER
#define SHADER
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "rt3d.h"

class Shader
{
public:
	Shader();

	GLuint init(char* vert, char* frag);

	void setMVP(GLuint shaderID, glm::mat4 stack);
	void setLight(GLuint shaderID, rt3d::lightStruct light);
	void setMaterial(GLuint shaderID, rt3d::materialStruct material);
	void setAttenuation(GLuint shaderID, float, float, float);
	void setProjection(GLuint shaderID, glm::mat4 projection);

	void setSpotlight(const GLuint program, rt3d::lightStruct spotLight, const GLfloat * direction);
	void setSpotlightAngles(const GLuint program, int angle1, int angle2);
	

private:
	//GLuint shaderID;
};



#endif
