#pragma once
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

	//Input
	void update();

	//Draw entire scene
	void draw();

	//Camera movement
	glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d);
	glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d);

	//Creates all the scene objects and then initialises all AABB's for those objects and gives them to the particle effect for collision testing
	void loadScene();

private:

	// light attenuation
	float attConstant;
	float attLinear;
	float attQuadratic;

	//Positional Light
	rt3d::lightStruct light = {
		{ 1.0f, 1.0f, 1.0f, 1.0f  },
		{ 1.0f, 1.0f, 1.0f, 1.0f  },
		{ 1.0f, 1.0f, 1.0f, 1.0f  },
		{ -15.0f, 5.0f, -30, 1.0f }
	};

	//Spot light
	rt3d::lightStruct spotLight = {
		{ 0.01f, 0.01f, 0.01f, 1.0f },
		{ 0.6f, 0.6f , 0.6f, 0.4f	},
		{ 0.6f, 0.6f, 0.6f, 0.4f	},
		{ -5.0f, 3.0f, -11.0f, 1.0f }
	};

	glm::vec4 spotLightPos;
	glm::vec3 spotLightDirection;
	int angle1;
	int angle2;

	//Material
	rt3d::materialStruct material = {
		{ 0.2f, 0.2f, 0.2f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		2.0f  // shininess
	};

	//Every Object In Our Scene
	std::vector<GameObject*> gameObjects;

	//Rendering
	Renderer *renderer;
	Shader shader;
	int shaderID[5];

	//Camera and View
	stack<glm::mat4> mvStack;
	GLfloat r;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
	glm::mat4 projection;

	//Particles
	ParticleEffect* Sprinkler;
	GLuint particleTexture;

};
