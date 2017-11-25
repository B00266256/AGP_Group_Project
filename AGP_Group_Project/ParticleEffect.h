#ifndef PARTICLE
#define PARTICLE

#include <glm/glm.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "rt3d.h"

class ParticleEffect
{
public:
	ParticleEffect(int n);
	~ParticleEffect();

	int getNoP() { return NoP; }
	void emitParticle();

	std::vector<glm::vec3> getPositions(void) const { return positions; }
	std::vector<glm::vec4> getColours(void) const { return colours; }
	std::vector<glm::vec3> getVel(void) const { return velocitys; }
	std::vector<glm::vec3> getAccel(void) const { return accel; }
	std::vector<float> lifespan;
	std::vector<bool> isAlive;

	void update(void);
	void draw(void);

private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec4> colours;
	std::vector<glm::vec3> velocitys;
	std::vector<glm::vec3> accel;

	glm::vec3  emitPosition;
	glm::vec3 bounceValue;

	int NoP;
	GLuint vao[1];
	GLuint vbo[2];

	double dt;
	int iterator;
};


#endif