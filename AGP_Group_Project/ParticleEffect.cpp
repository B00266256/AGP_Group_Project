#include "ParticleEffect.h"
#include <math.h>

#define DEG_TO_RADIAN 0.017453293

int rando(int max, int min) //Pass in range
{
	srand(time(0));  //Changed from rand(). srand() seeds rand for you.
	int random = rand() % max + min;
	return random;
}


ParticleEffect::ParticleEffect(int numOfParticles, glm::vec3 pos) : NoP(numOfParticles)
{
	// Init values
 collisionTester = new CollisionTest();
	srand(time(0));
	emitPosition = pos;
	bounceValue = glm::vec3(0, 0.1, 0);
	iterator = 0;
	acceleration = glm::vec3(0, -80, 0);
	start = 0;
	lifespan = 750;
	bounceLifespan = 90;

	// Initialise all particles
	for (int i = 0; i < NoP; i++)
	{
	
		positions.push_back(emitPosition);
		velocitys.push_back(glm::vec3(0,0,0));
		colours.push_back(glm::vec4(0, 0, 1, 1));
		accel.push_back(glm::vec3(0, 0, 0));
		isAlive.push_back(false);
		age.push_back(lifespan);

	}

	//init vao/vbo
	glGenVertexArrays(1, vao);
	glGenBuffers(2, vbo);
	glBindVertexArray(vao[0]); // bind VAO 0 as current object

							   // Position data in attribute index 0, 3 floats per vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO for positions
	glBufferData(GL_ARRAY_BUFFER, NoP  * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index 0
	 
									  // Colours data in attribute 1, 3 floats per vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // bind VBO for colours
	glBufferData(GL_ARRAY_BUFFER, NoP  * sizeof(glm::vec4), colours.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);    // Enable attribute index 1
	glBindVertexArray(0);
}

ParticleEffect::~ParticleEffect()
{
	//delete positions;
	//delete colours;
	//delete velocity;
}




int k;


void ParticleEffect::emitParticle()
{
	// Iterate over and re-emit particles
	if (iterator != NoP)
	{
		// Calculate
		float circleSegment = (360.0f / NoP);
		float angle = (circleSegment * iterator);
		float initialSpeed = rand() % 111 / 20;
		glm::vec3 launchVelocity = glm::vec3(0.0f + (initialSpeed * std::cos(angle)), 0.0f, 0.0f + (initialSpeed * std::sin(angle)));


		// Reset particle data
		positions[iterator] = emitPosition;
		velocitys[iterator] = glm::vec3(launchVelocity);
		accel[iterator] = acceleration;
		isAlive[iterator] = true;
		age[iterator] = lifespan;

		// Set 50% of particle slightly lower alpha values. Very messy, sorry not sorry.
		if (NULL)
			k = 0;

		if (k == 1)
		{
			colours[iterator] = glm::vec4(0.8, 0.8, 1.0, 0.6);
			k = 0;
		}
		else
		{
			colours[iterator] = glm::vec4(0.5, 0.5, 0.8, 0.5);
			k = 1;
		}


		iterator++;
	}
	else
	{	//we've emitted all particles(NoP), start re-emitting from particle 0
		iterator = 0;
	}
		

}

void ParticleEffect::update()
{
	double dt = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	

	// Initialise next particle
	emitParticle();
		
	glm::vec3 min(-16, -8, -31);
	glm::vec3 max(-14, -6, -29);

		// Iterate over all particles
		for (int i = 0; i < NoP; i++)
		{
				// If particle has expired has reached end of life mark as dead or decrement lifespan
			if (age[i] <= 0)
			{
				isAlive[i] = false;
				positions[i] = emitPosition;
			}
			else if(age[i] != 0 && isAlive[i])
			{
				age[i] -= 1;
			}
			
			
			
			// If the particle has not hit the ground yet and alive
			if (positions[i].y >= -9.5 && isAlive[i])
			{		
				positions[i] = positions[i] + (glm::vec3(dt)*velocitys[i]) + glm::vec3(0.5f*(dt*dt))*accel[i];

				velocitys[i] = velocitys[i] + (glm::vec3(0.5f*dt)*accel[i]);	
			}
			else //Particle has hit the ground - bounce partiicle
			{	
				positions[i].y = -9.4;
				velocitys[i] = -velocitys[i] * (bounceValue);
				age[i] = bounceLifespan;
			}

			//for all possible collidables check for collision
			for (int j = 0; j < collidableObjects.size(); j++)
			{
				if (collisionTester->AABBtoPoint(collidableObjects[j]->getMin(), collidableObjects[j]->getMax(), positions[i]) == true)
				{
					/*std::vector<glm::vec3> aabb;
					aabb.push_back(min);
					aabb.push_back(min + glm::vec3(0, 0, 2));
					aabb.push_back(min + glm::vec3(2, 0, 0));*/

					//glm::vec3 normal = collisions->getCollisionNormal(aabb);
					glm::vec3 normal(0, 1, 0);

					//glm::vec3 bounceDirection = normal * glm::vec3(bounceValue.y);

					//Addjusts height since the particle is probably partially inside the collideable object
					positions[i].y += 0.2;

					//reflect upwards and dampen velocity
					velocitys[i] = -velocitys[i] * (bounceValue);

					//reflect in respect to the surface norm
					velocitys[i] += normal;

					//Sets the lifespan so it will die shortly after bounce
					age[i] = bounceLifespan;
				
					//slightly increase alpha
					colours[i].a += 0.1;
				}
			}

			
	
		}
	
		start = std::clock();
}





void ParticleEffect::draw()
{
	glBindVertexArray(vao[0]); // bind VAO 0 as current object

	// particle data may have been updated - so need to resend to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO 0
	glBufferData(GL_ARRAY_BUFFER, NoP * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);

	// Position data in attribute index 0, 3 floats per vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index 


	//update colours
	glBindVertexArray(vao[1]); // bind VAO 0 as current object
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // bind VBO 0
	glBufferData(GL_ARRAY_BUFFER, NoP * sizeof(glm::vec4), colours.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index 


	// Now draw the particles... as easy as this!
	glDrawArrays(GL_POINTS, 0, NoP);
	glBindVertexArray(0);




}