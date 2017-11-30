#pragma once
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "GameObject.h"
#include "Shader.h"
#include <stack>

#define DEG_TO_RADIAN 0.017453293

class Renderer
{
public:

	//Stack of GameObjects taht make up the whole scene and the model view with latest camera data
	void draw(vector<GameObject*> drawList, stack<glm::mat4> mvStack);

private:
	Shader shader;
};



