#pragma once
#include <glm/glm.hpp>
#include <vector>


class CollisionTest
{
public:
	CollisionTest(){}
	~CollisionTest(){}

	bool AABBtoPoint(glm::vec3 min, glm::vec3 max, glm::vec3 point);	//Uses Seperatin Axis Thereom to find if there is a collision between a given AABB and a point(particle).
	bool checkCollectableCollision(glm::vec3 userPos, glm::vec3 collectablePos);	//A simple distance check method that returns true when the user is near the collectable item gameplay element.
	float distanceTo(glm::vec3 pos1, glm::vec3 pos2);

	glm::vec3 getCollisionNormal(std::vector<glm::vec3> face); //Returns the collison normal of a mesh face;
};