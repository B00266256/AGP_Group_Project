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




	void loadGroundAndWalls();

private:
	rt3d::lightStruct light = {
		{ 0.3f, 0.3f, 0.3f, 1.0f }, // ambient
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
		{ -10.0f, 10.0f, 10.0f, 1.0f }  // position
	};

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

	ParticleEffect* particleTest;
	GLuint particleTexture;

};
#endif