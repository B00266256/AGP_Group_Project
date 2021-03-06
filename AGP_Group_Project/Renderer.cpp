#include "Renderer.h"

void Renderer::draw(vector<GameObject*> drawList, stack<glm::mat4> mvStack)
{
	//for every gameObject draw all of their instances
	for (unsigned int i = 0; i < drawList.size(); i++) {

		for (unsigned int j = 0; j < drawList[i]->getInstanceCount(); j++)
		{

			glUseProgram(drawList[i]->getShader());
			glBindTexture(GL_TEXTURE_2D, drawList[i]->getTexID());

			mvStack.push(mvStack.top());

			//translate
			mvStack.top() = glm::translate(mvStack.top(), drawList[i]->getPos(j));
			//rotate
			if (drawList[i]->getRotate() != glm::vec3(NULL, NULL, NULL))
				mvStack.top() = glm::rotate((mvStack.top()), float(drawList[i]->getRotationValue() * DEG_TO_RADIAN), drawList[i]->getRotate());
			//scale
			mvStack.top() = glm::scale(mvStack.top(), drawList[i]->getScaling());

			shader.setMVP(drawList[i]->getShader(), mvStack.top());
			rt3d::drawIndexedMesh(drawList[i]->getMeshID(), drawList[i]->getMeshIndexCount(), GL_TRIANGLES);

			mvStack.pop();

		}
	}
}