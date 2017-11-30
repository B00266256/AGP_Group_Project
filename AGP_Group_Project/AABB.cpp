#include "AABB.h"


AABB::AABB(std::vector<glm::vec3> positions, glm::vec3 scale)
{
	//Just initialise the values before testing
	glm::vec3 minimum(200, 200, 200);
	glm::vec3 maximum(-200, -200, -200);
	

		//Extract the min and max instance of the object. Really just for the floor since it has multiple meshes being rendered.
		for (int i = 0; i < positions.size(); i++)
		{
			if (positions[i].x < minimum.x  &&		
				positions[i].y < minimum.y  &&		
				positions[i].z < minimum.z) 		
				minimum = positions[i];

			if (positions[i].x > maximum.x  &&		
				positions[i].y > maximum.y  &&		
				positions[i].z > maximum.z) 		
				maximum = positions[i];

		}

		//sets min and max taking into account the scaling of the object
		this->min = minimum - scale * glm::vec3(positions.size());
		this->max = maximum +  scale * glm::vec3(positions.size());

		
}




glm::vec3 AABB::getMin() {
	return min;
}


glm::vec3 AABB::getMax() { 
	return max; 
}