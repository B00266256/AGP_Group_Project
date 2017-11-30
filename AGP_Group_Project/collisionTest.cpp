#include "collisionTest.h"

// Fail fast SAT test
bool CollisionTest::AABBtoPoint(glm::vec3 min, glm::vec3 max, glm::vec3 point)
{

	if (point.x < min.x || point.x > max.x)  return false;
	if (point.y < min.y || point.y > max.y)  return false;
	if (point.z < min.z || point.z > max.z)  return false;
	
		return true;
	
}

bool CollisionTest::checkCollectableCollision(glm::vec3 userPos, glm::vec3 collectablePos)
{
	if (distanceTo(userPos, collectablePos) <= 3.0f)
		return true;
	return false;
}

float CollisionTest::distanceTo(glm::vec3 pos1, glm::vec3 pos2)
{
	return sqrt(((pos2.x - pos1.x) * (pos2.x - pos1.x)) + ((pos2.y - pos1.y) * (pos2.y - pos1.y)) + ((pos2.z - pos1.z) * (pos2.z - pos1.z)));
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

