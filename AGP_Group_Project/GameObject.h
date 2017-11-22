#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <glm/glm.hpp>
#include "rt3d.h"
#include "TextureUtils.h"
#include "rt3dObjLoader.h"
#include <vector>
using namespace std;

class GameObject
{
public:
	GameObject(glm::vec3 pos, glm::vec3 scale, glm::vec3 rota);

	void addObjectInstance(glm::vec3 pos);

	void setTexture(char* textureFile);
	void setMaterial(rt3d::materialStruct mat);
	void setMesh(char* objectFile);
	void setShader(GLuint shaderID);


	GLuint getTexID();
	GLuint getMeshID();
	GLuint getShader();

	glm::vec3 getPos(int i);
	glm::vec3 getScaling();
	glm::vec3 getRotate();

	int getInstanceCount();
	int getMeshIndexCount();
	
private:
	rt3d::materialStruct material;

	vector<glm::vec3> positions;
	int numberOfInstances;

	glm::vec3 position;
	glm::vec3 scaling;
	glm::vec3 rotation;

	GLuint mesh;
	GLuint texture;
	GLuint shader;

	int meshIndexCount;

};


#endif