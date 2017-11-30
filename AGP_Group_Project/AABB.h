#pragma once
#include <glm/glm.hpp>
#include <vector>


class AABB
{
public:
	//Uses the objects position in worldspace and its scale to generate its min and max values
	AABB(std::vector<glm::vec3> positions, glm::vec3 scale);

	glm::vec3 getMin();
	glm::vec3 getMax();
	
private:
	glm::vec3 min;
	glm::vec3 max;
};
