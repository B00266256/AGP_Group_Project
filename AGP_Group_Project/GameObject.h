//GameObject.h
//AGP Group Project B00266256, B00286864.
//"Used to define the physical objects present in our scene."

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
	
	GameObject();													//Constructor - Default.
	GameObject(glm::vec3 scale, glm::vec3 rota);					//Constructor
	GameObject(glm::vec3 scale, glm::vec3 rota, float rotdegree);	//Constructor

	
	void initAABB();						//generate an AABB using objects position(s) and scaling.
	void addObjectInstance(glm::vec3 pos);	//add another intance of the model, its mesh will be rendered as well. (Really just the floor and walls use this).

	//Set methods for private variables...
	void setTexture(char* textureFile);
	void setMaterial(rt3d::materialStruct mat);
	void setMesh(char* objectFile);
	void setShader(GLuint shaderID);

	//Get methods for private variables...
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
	AABB* aabb; //an axis alligned bounding box for the object -if initialised.

	vector<glm::vec3> positions;	//positions of all object instances.
	glm::vec3 scaling;				//size of the object.
	glm::vec3 rotation;				//rotation vector of the object.
	float rotationInDeg;			//value of the rotation in degrees.
	int numberOfInstances;			//number of instances of this object in the scene.

	//Renderer values...
	rt3d::materialStruct material;	//object material.
	GLuint texture;					//object texture.
	GLuint shader;					//shader program to be used.
	GLuint mesh;					//object mesh.
	vector<GLfloat> verts;			//number of vertices.
	int meshIndexCount;				//number of indices.
};


