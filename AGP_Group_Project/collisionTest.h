#pragma once
#include <glm/glm.hpp>
#include <vector>



class CollisionTest
{
public:
	CollisionTest(){}
	glm::vec3  getCollisionNormal(std::vector<glm::vec3> face);
	glm::vec3 PointToFace(glm::vec3 point, std::vector<glm::vec3> face);


	bool AABBtoPoint(glm::vec3 min, glm::vec3 max, glm::vec3 point);

private:


};