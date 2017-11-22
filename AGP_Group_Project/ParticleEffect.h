#ifndef PARTICLE
#define PARTICLE

#include <glm/glm.hpp>
#include <vector>
#include <cstdlib>
#include "rt3d.h"

class ParticleEffect
{
public:
	ParticleEffect(int n);
	~ParticleEffect();

	int getNoP() { return NoP; }

	std::vector<glm::vec3> getPositions(void) const { return positions; }
	std::vector<glm::vec3> getColours(void) const { return colours; }
	std::vector<glm::vec3> getVel(void) const { return velocity; }

	void update(void);
	void draw(void);

private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colours;
	std::vector<glm::vec3> velocity;

	std::vector<glm::vec3> acceleration;

	int NoP;
	GLuint vao[1];
	GLuint vbo[2];
};


#endif