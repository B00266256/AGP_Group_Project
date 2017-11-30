#pragma once

#include <glm/glm.hpp>
#include "rt3d.h"
#include "TextureUtils.h"
#include "rt3dObjLoader.h"
#include <vector>
#include "AABB.h"
using namespace std;

class GameObject
{
public:
	// Constructors
	GameObject(glm::vec3 pos, glm::vec3 scale, glm::vec3 rota);
	GameObject(glm::vec3 pos, glm::vec3 scale, glm::vec3 rota, float rotdegree);

	// Generate aabb using objects position(s) and scaling
	void initAABB();

	// Add a new position and another intance of the model will be rendered there as well. (Really just the floor and walls use this).
	void addObjectInstance(glm::vec3 pos);

	// Set up of render data
	void setTexture(char* textureFile);
	void setMaterial(rt3d::materialStruct mat);
	void setMesh(char* objectFile);
	void setShader(GLuint shaderID);

	// Gets
	GLuint getTexID();
	GLuint getMeshID();
	GLuint getShader();

	glm::vec3 getPos(int i);
	glm::vec3 getScaling();
	glm::vec3 getRotate();
	float GameObject::getRotationValue();

	glm::vec3 getMin();
	glm::vec3 getMax();

	int getInstanceCount();
	int getMeshIndexCount();
	
private:
	// Collisions
	AABB* aabb;

	// Positional Data
	vector<glm::vec3> positions;
	glm::vec3 scaling;
	glm::vec3 rotation;
	float rotationInDeg;

	int numberOfInstances;

	// Render Data
	rt3d::materialStruct material;
	GLuint texture;
	GLuint shader;

	GLuint mesh;
	vector<GLfloat> verts;
	int meshIndexCount;

};


