#ifndef PARTICLE
#define PARTICLE

#include <glm/glm.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "rt3d.h"
#include "collisionTest.h"
#include "GameObject.h"

class ParticleEffect
{
public:
	ParticleEffect(int n, glm::vec3 pos);
	~ParticleEffect();

	int getNoP() { return NoP; }
	void emitParticle();

	std::vector<glm::vec3> getPositions(void) const { return positions; }
	std::vector<glm::vec4> getColours(void) const { return colours; }
	std::vector<glm::vec3> getVel(void) const { return velocitys; }
	std::vector<glm::vec3> getAccel(void) const { return accel; }
	std::vector<float> age;
	std::vector<bool> isAlive;

	void addtCollidableObjects(GameObject * object) { collidableObjects.push_back(object);  }
	void setEmitPos(glm::vec3 pos);

	void init(void);
	void update(void);
	void draw(void);

private:
	CollisionTest *collisionTester;
	std::vector<GameObject*> collidableObjects;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec4> colours;
	std::vector<glm::vec3> velocitys;
	std::vector<glm::vec3> accel;
	std::vector<GLuint> texID;

	glm::vec3  emitPosition;				//launch position (sprinkler head)
	glm::vec3 bounceValue;					// how much the velocity gets dampened when hitting the ground
	glm::vec3 acceleration;					// Applied to particles that are alive
	int lifespan;
	int bounceLifespan;						// Its life span once it bounced

	int NoP;
	GLuint vao[1];
	GLuint vbo[2];

	int iterator;							//iterates over all the particles when emitting them

	std::clock_t start;
	double dt;

	GLuint launchTexture;
	GLuint collideTexture;

};


#endif