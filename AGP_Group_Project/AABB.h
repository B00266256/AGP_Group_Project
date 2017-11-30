#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "rt3d.h"

class AABB
{
public:
	AABB(std::vector<glm::vec3> positions, glm::vec3 scale);

	glm::vec3 getMin();
	glm::vec3 getMax();
	
	glm::vec3 min;
	glm::vec3 max;
private:
	
};
