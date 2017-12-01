//AABB.h
//AGP Group Project B00266256, B00286864.
//"Used to define an Axis-Aligned-Bounding-Boxes for particle collision detection."

#pragma once
#include <glm/glm.hpp>
#include <vector>

class AABB
{
public:	
	AABB(std::vector<glm::vec3> positions, glm::vec3 scale); //Constructor - uses position in worldspace and scale to generate min and max values.

	//Get methods for private values...
	glm::vec3 getMin();
	glm::vec3 getMax();
	
private:
	glm::vec3 min; //AABB minimum value.
	glm::vec3 max; //AABB maximum value.
};
