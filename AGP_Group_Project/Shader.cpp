#include "Shader.h"



Shader::Shader()
{

}

GLuint Shader::init(char* vert, char* frag)
{
	return rt3d::initShaders(vert, frag);
}

void Shader::setProjection(GLuint shaderID, glm::mat4 projection)
{
	rt3d::setUniformMatrix4fv(shaderID, "projection", glm::value_ptr(projection));
}

void Shader::setMVP(GLuint shaderID, glm::mat4 stack)
{
	rt3d::setUniformMatrix4fv(shaderID, "modelview", glm::value_ptr(stack));
}

void Shader::setLight(GLuint shaderID, rt3d::lightStruct light)
{
	rt3d::setLight(shaderID, light);
}

void Shader::setSpotlight(const GLuint program, rt3d::lightStruct spotLight, const GLfloat * direction)
{
	rt3d::setSpotlightUniforms(program, spotLight, direction);
}

void Shader::setMaterial(GLuint shaderID, rt3d::materialStruct material)
{
	rt3d::setMaterial(shaderID, material);
}

void Shader::setAttenuation(GLuint shaderID, float attConstant, float attLinear, float attQuadratic)
{
	GLuint uniformIndex = glGetUniformLocation(shaderID, "attConst");
	glUniform1f(uniformIndex, attConstant);
	uniformIndex = glGetUniformLocation(shaderID, "attLinear");
	glUniform1f(uniformIndex, attLinear);
	uniformIndex = glGetUniformLocation(shaderID, "attQuadratic");
	glUniform1f(uniformIndex, attQuadratic);

}