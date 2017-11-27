#pragma once
#include "AABB.h"

using namespace std;
#include <glm/gtc/matrix_transform.hpp>
#define DEG_TO_RADIAN 0.017453293


AABB::AABB(std::vector<glm::vec3> positions, glm::vec3 scale)
{
	//Just initialise the values before testing
	glm::vec3 minimum(200, 200, 200);
	glm::vec3 maximum(-200, -200, -200);
	

	//Extract the min and max 
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

		glm::vec3 texMax = this->max;
		glm::vec3 texMin = glm::vec3(min.x, this->max.y, min.z);

	/*	unsigned char pixel[4];
		glReadPixels(0.5, 0.5, 1, 1, GL_RGB, GL_BITMAP, pixel);
		cout << "R: " << (float)pixel[0] << endl;
		cout << "G: " << (float)pixel[1] << endl;
		cout << "B: " << (float)pixel[2] << endl;
		cout << endl;*/

		texMin -= texMin;
		texMax -= texMin;
}




glm::vec3 AABB::getMin() { return min; }
glm::vec3 AABB::getMax() { return max; }