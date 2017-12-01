//Scene.h
//AGP Group Project B00266256, B00286864.
//"Used to define, draw -via calls- and update our Scene."

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
	Scene();	//Constructor - Default.
	~Scene();	//Destructor.

	//Default methods...
	void init();	//initialises our scene and its objects.
	void update();	//updates the scene with the current values.
	void draw();	//draws the scene to the window.

	//Camera movement...
	glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d);
	glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d);

	void loadScene();	//Loads all scene objects, initialises any of their AABB's and passes them to the particle system for collision testing.

private:

	//Light attenuation values...
	float attConstant;
	float attLinear;
	float attQuadratic;

	//Positional Light...
	rt3d::lightStruct light = {
		{ 1.0f, 1.0f, 1.0f, 1.0f  },
		{ 1.0f, 1.0f, 1.0f, 1.0f  },
		{ 1.0f, 1.0f, 1.0f, 1.0f  },
		{ -15.0f, 5.0f, -30, 1.0f }
	};

	//Spot Light...
	rt3d::lightStruct spotLight = {
		{ 0.01f, 0.01f, 0.01f, 1.0f },
		{ 0.5f, 0.5f , 0.5f, 0.4f	},
		{ 0.5f, 0.5f, 0.5f, 0.4f	},
		{ -5.0f, 3.0f, -11.0f, 1.0f }
	};

	//Spotlight values used in updating and drawing...
	glm::vec4 spotLightPos;
	glm::vec3 spotLightDirection;
	int angle1;
	int angle2;

	//Default Material...
	rt3d::materialStruct material = {
		{ 0.2f, 0.2f, 0.2f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		2.0f  // shininess
	};

	//Camera/Modelview/Projection values...
	stack<glm::mat4> mvStack;
	GLfloat r;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
	glm::mat4 projection;

	//Rendering System et al...
	Renderer *renderer;
	Shader shader;
	int shaderID[5];

	//Particle System et al...
	ParticleEffect* Sprinkler;
	GLuint particleTexture;

	std::vector<GameObject*> gameObjects; //vector of all objects in the scene.
};
