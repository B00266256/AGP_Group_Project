#include "GameObject.h"



GameObject::GameObject(glm::vec3 pos, glm::vec3 scale, glm::vec3 rota)
{
	position = pos;
	scaling = scale;
	rotation = rota;

	
}

void GameObject::initAABB()
{
	// Generate aabb
	aabb = new AABB(positions, scaling);
}

glm::vec3 GameObject::getMin()
{
	return aabb->min;
}

glm::vec3 GameObject::getMax()
{
	return aabb->max;
}

void GameObject::addObjectInstance(glm::vec3 pos)
{
	positions.push_back(pos);
	numberOfInstances++;
}


void GameObject::setTexture(char* textureFile)
{
	texture = TextureUtils::loadBitmap(textureFile);
}

void GameObject::setMaterial(rt3d::materialStruct mat)
{
	material = mat;
}

void GameObject::setMesh(char* objectFile)
{
	
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj(objectFile, verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;

	mesh = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
}

void GameObject::setShader(GLuint shaderID)
{
	shader = shaderID;
}





GLuint GameObject::getTexID() { return texture; }
GLuint GameObject::getMeshID() { return mesh; }
GLuint GameObject::getShader() { return shader; }

glm::vec3 GameObject::getPos(int i)
{ 
	return positions[i]; 
}


glm::vec3 GameObject::getScaling() { return scaling; }
glm::vec3 GameObject::getRotate() { return rotation; }

int GameObject::getMeshIndexCount() { return meshIndexCount; }
int GameObject::getInstanceCount() { return numberOfInstances; }