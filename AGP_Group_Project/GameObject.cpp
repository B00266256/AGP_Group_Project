#include "GameObject.h"


GameObject::GameObject()
{
	scaling = glm::vec3(1.0f,1.0f,1.0f);
	rotation = glm::vec3(0.0f,0.0f,0.0f);
	rotationInDeg = 0.0f;
}

GameObject::GameObject(glm::vec3 scale, glm::vec3 rota)
{
	scaling = scale;
	rotation = rota;
	rotationInDeg = 0.0f;
}

GameObject::GameObject(glm::vec3 scale, glm::vec3 rota, float deg)
{
	scaling = scale;
	rotation = rota;
	rotationInDeg = deg;
}

void GameObject::initAABB()
{
	aabb = new AABB(positions, scaling);
}

void GameObject::addObjectInstance(glm::vec3 pos)
{
	positions.push_back(pos);
	numberOfInstances++;
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

// Sets
void GameObject::setTexture(char* textureFile) { texture = TextureUtils::loadBitmap(textureFile); }

void GameObject::setMaterial(rt3d::materialStruct mat) { material = mat; }

void GameObject::setShader(GLuint shaderID) { shader = shaderID; }


// Gets
glm::vec3 GameObject::getMin()		{ return aabb->getMin();	}
glm::vec3 GameObject::getMax()		{ return aabb->getMax();	}
GLuint GameObject::getTexID()		{ return texture;			}
GLuint GameObject::getMeshID()		{ return mesh;				}
GLuint GameObject::getShader()		{ return shader;			}
glm::vec3 GameObject::getPos(int i) { return positions[i];		}
glm::vec3 GameObject::getScaling()	{ return scaling;			}
glm::vec3 GameObject::getRotate()	{ return rotation;			}
float GameObject::getRotationValue(){ return rotationInDeg;		}
int GameObject::getMeshIndexCount() { return meshIndexCount;	}
int GameObject::getInstanceCount()	{ return numberOfInstances; }