#ifndef SCENE
#define SCENE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "GameObject.h"
#include "Shader.h"
#include "rt3d.h"
#include <stack>
#include "Renderer.h"
#include "ParticleEffect.h"

using namespace std;
#define DEG_TO_RADIAN 0.017453293

class Scene
{
public:
	Scene();
	void init();

	void update();
	void draw();

	glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d);
	glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d);




	void loadScene();

private:

	// light attenuation
	float attConstant;
	float attLinear;
	float attQuadratic;

	rt3d::lightStruct light = {
		{ 1.0f, 1.0f, 1.0f, 1.0f },		// ambient
		{ 1.0f, 1.0f, 1.0f, 1.0f },		// diffuse
		{ 1.0f, 1.0f, 1.0f, 1.0f },		// specular
		{ -15.0f, 5.0f, -30, 1.0f }		 // position
	};

	//Spot light
	rt3d::lightStruct spotLight = {
		{ 0.01f, 0.01f, 0.01f, 1.0f },			// ambient
		{ 0.6f, 0.6f , 0.6f, 0.4f },				// diffuse
		{ 0.6f, 0.6f, 0.6f, 0.4f },				// specular
		{ -5.0f, 3.0f, -11.0f, 1.0f }			// position
	};

	glm::vec4 spotLightPos;
	glm::vec3 spotLightDirection;
	int angle1;												
	int angle2;

	rt3d::materialStruct material = {
		{ 0.2f, 0.2f, 0.2f, 1.0f }, // ambient
		{ 0.5f, 0.5f, 0.5f, 1.0f }, // diffuse
		{ 0.0f, 0.0f, 0.0f, 1.0f }, // specular
		2.0f  // shininess
	};

	std::vector<GameObject*> gameObjects;
	Renderer *renderer;

	Shader shader;
	int shaderID[5];

	stack<glm::mat4> mvStack;
	GLfloat r;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
	glm::mat4 projection;

	ParticleEffect* Sprinkler;
	GLuint particleTexture;

};
#endif