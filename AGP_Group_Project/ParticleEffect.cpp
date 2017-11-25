#include "ParticleEffect.h"
#include <math.h>

#define DEG_TO_RADIAN 0.017453293

int rando(int max, int min) //Pass in range
{
	srand(time(0));  //Changed from rand(). srand() seeds rand for you.
	int random = rand() % max + min;
	return random;
}


ParticleEffect::ParticleEffect(int numOfParticles) : NoP(numOfParticles)
{
	//init values
	srand(time(0));
	emitPosition = glm::vec3(-15, 10, -30);
	bounceValue = glm::vec3(0, 0.2, 0);
	iterator = 0;

	for (int i = 0; i < NoP; i++)
	{
	
		positions.push_back(emitPosition);
		velocitys.push_back(glm::vec3(0,0,0));
		colours.push_back(glm::vec4(0, 0, 1, 1));
		accel.push_back(glm::vec3(0, 0, 0));
		isAlive.push_back(false);

		lifespan.push_back(800);

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


std::clock_t start = 0;

int k;


void ParticleEffect::emitParticle()
{
	//emit the next particle
	if (iterator != NoP)
	{

		float circleSegment = (360.0f / NoP);
		float angle = (circleSegment * iterator);
		float initialSpeed = rand() % 111 / 20;
		glm::vec3 initialPosition = emitPosition;

		glm::vec3 launchVelocity = glm::vec3(0.0f + (initialSpeed * std::cos(angle)), 0.0f, 0.0f + (initialSpeed * std::sin(angle)));

		positions[iterator] = initialPosition;
		velocitys[iterator] = glm::vec3(launchVelocity);
		accel[iterator] = glm::vec3(0, -80, 0);
		isAlive[iterator] = true;
		lifespan[iterator] = 800;

		//set 50% of particle slightly lower alpha values. Very messy, sorry not sorry.
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
	

	//initialise next particle
	emitParticle();
		
		for (int i = 0; i < NoP; i++)
		{
			
				//if particle has expiredhas reached end of life mark as dead
				if (lifespan[i] <= 0)
				{
					isAlive[i] = false;
				}
				else if(lifespan[i] != 0 && isAlive[i])
				{
					lifespan[i] -= 1;
				}
			

			// If not hit the ground yet and marked as alive
			if (positions[i].y >= -9.5 && isAlive[i])
			{		
					positions[i] = positions[i] + (glm::vec3(dt)*velocitys[i]) + glm::vec3(0.5f*(dt*dt))*accel[i];

					velocitys[i] = velocitys[i] + (glm::vec3(0.5f*dt)*accel[i]);	
			}
			else
			{	//Particle has hit the ground - bounce partiicle
				positions[i].y = -9.4;
				velocitys[i] = -velocitys[i] * (bounceValue);
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
	//glPointSize(5);
	glDrawArrays(GL_POINTS, 0, NoP);
	glBindVertexArray(0);




}