#pragma once
#include <glm/glm.hpp>
#include <vector>


class CollisionTest
{
public:
	CollisionTest(){}

	//Uses Seperatin Axis Thereom to find if there is a collision between a given AABB and a point(particle)
	bool AABBtoPoint(glm::vec3 min, glm::vec3 max, glm::vec3 point);

	glm::vec3  getCollisionNormal(std::vector<glm::vec3> face);

};