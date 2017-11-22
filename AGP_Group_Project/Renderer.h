#ifndef RENDER
#define RENDER
#define DEG_TO_RADIAN 0.017453293

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "GameObject.h"
#include "Shader.h"
#include <stack>

class Renderer
{
public:
	Renderer();

	void draw(vector<GameObject*> drawList, stack<glm::mat4> mvStack);

private:
	Shader shader;
};



#endif
