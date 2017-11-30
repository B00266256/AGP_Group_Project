#include "collisionTest.h"

// Fail fast SAT test
bool CollisionTest::AABBtoPoint(glm::vec3 min, glm::vec3 max, glm::vec3 point)
{

	if (point.x < min.x || point.x > max.x)  return false;
	if (point.y < min.y || point.y > max.y)  return false;
	if (point.z < min.z || point.z > max.z)  return false;
	
		return true;
	
}

//This was to extend the collsiion reaction to using surface normals. Not completed/used
glm::vec3 CollisionTest::getCollisionNormal( std::vector<glm::vec3> face)
{
	glm::vec3 v1 = face[0];
	glm::vec3 v2 = face[1];
	glm::vec3 v3 = face[2];

	glm::vec3 faceNormal = glm::cross((v2 - v1), (v3 - v1));
	faceNormal = glm::normalize(faceNormal);

	return faceNormal;
}

