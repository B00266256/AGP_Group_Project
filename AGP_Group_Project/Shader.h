//Scene.h
//AGP Group Project B00266256, B00286864.
//"A facade class to interface with rt3d shader functions and pass values to the shaders."

#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "rt3d.h"

class Shader
{
public:
	Shader(){}	//Constructor - Default.
	~Shader(){}	//Destructor.
	
	GLuint init(char* vert, char* frag); //initialise a shader pair and pass out its ID.

	//Updates values for a given shader...
	void setMVP(GLuint shaderID, glm::mat4 stack);
	void setLight(GLuint shaderID, rt3d::lightStruct light);
	void setMaterial(GLuint shaderID, rt3d::materialStruct material);
	void setAttenuation(GLuint shaderID, float, float, float);
	void setProjection(GLuint shaderID, glm::mat4 projection);
	void setSpotlight(const GLuint program, rt3d::lightStruct spotLight, const GLfloat * direction);
	void setSpotlightAngles(const GLuint program, int angle1, int angle2);
	void setParticleSize(const GLuint program, int size);

};

