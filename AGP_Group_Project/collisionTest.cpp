#include "collisionTest.h"
#include <iostream>

bool CollisionTest::AABBtoPoint(glm::vec3 min, glm::vec3 max, glm::vec3 point)
{
	/*if (min.x > point.x && max.x < point.x)	return false;
	if (min.y > point.y && max.y < point.y)	return false;
	if (min.z > point.z && max.z < point.z)	return false;
*/
	/*if ( (point.x   < min.x && point.x > max.x) &&	
		 (point.y < min.y && point.y > max.y) &&
		 (point.z < min.z && point.z > max.z) )	
		return false;*/

	//glm::vec3 pointMax = point + glm::vec3(1);
	//glm::vec3 pointMin = point - glm::vec3(1);
	//float pointMin = point.x - 0.1;

	/*if ((pointMax.x > min.x && pointMin.x < max.x) &&
		(pointMax.y > min.y && pointMin.y < max.y) &&
		(pointMax.z > min.z && pointMin.z < max.z))
	{
		std::cout << "Collision" << std::endl;
		return true;
	}*/

	if ((point.x > min.x && point.x < max.x) &&
		(point.y > min.y && point.y < max.y) &&
		(point.z > min.z && point.z < max.z))
	{
		std::cout << "Collision" << std::endl;
		return true;
	}
		

	


	/*tBox1.m_vecMax.x > tBox2.m_vecMin.x &&
		tBox1.m_vecMin.x < tBox2.m_vecMax.x &&

		tBox1.m_vecMax.y > tBox2.m_vecMin.y &&
		tBox1.m_vecMin.y < tBox2.m_vecMax.y &&

		tBox1.m_vecMax.z > tBox2.m_vecMin.z &&
		tBox1.m_vecMin.z < tBox2.m_vecMax.z);*/

	return false;
}



glm::vec3 CollisionTest::getCollisionNormal( std::vector<glm::vec3> face)
{

	glm::vec3 v1 = face[0];
	glm::vec3 v2 = face[1];
	glm::vec3 v3 = face[2];

	glm::vec3 faceNormal = glm::cross((v2 - v1), (v3 - v1));
	faceNormal = glm::normalize(faceNormal);

	return faceNormal;
}


glm::vec3 CollisionTest::PointToFace(glm::vec3 point, std::vector<glm::vec3> face)
{
	// get distance from point to plane
	//if 0 then its colliding
	//get surface normal and return it

	

		


	return glm::vec3(1, 1, 1);
}