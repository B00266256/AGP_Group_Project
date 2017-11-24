#include "ParticleEffect.h"
#include <math.h>

#define DEG_TO_RADIAN 0.017453293

int rando(int max, int min) //Pass in range
{
	srand(time(NULL));  //Changed from rand(). srand() seeds rand for you.
	int random = rand() % max + min;
	return random;
}


ParticleEffect::ParticleEffect(int numOfParticles) : NoP(numOfParticles)
{
	//init values
	srand(time(0));
	accel = glm::vec3(0, -9.8, 0);

	for (int i = 0; i < NoP; i++)
	{
		float circleSegment = (360.0f / NoP);
		float angle = (circleSegment * i);
		float initialSpeed = 1.5;
		glm::vec3 initialPosition = glm::vec3(-15, 2, -30);

		glm::vec3 launchVelocity = glm::vec3(0.0f + (initialSpeed * std::cos(angle)), 0.0f, 0.0f + (initialSpeed * std::sin(angle)));


		positions.push_back(initialPosition);
		velocitys.push_back(launchVelocity);
		colours.push_back(glm::vec3(0, 0, 1));

		lifespan.push_back(450);

		std::cout << rando(1, 3) << std::endl;

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
	glBufferData(GL_ARRAY_BUFFER, NoP  * sizeof(glm::vec4), colours.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
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

void ParticleEffect::update()
{

	double dt = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	float incre = 0.0;
		//Verlet Solver
		

		for (int i = 0; i < NoP; i++)
		{

			if (lifespan[i] == 0)
				{
					positions[i] = glm::vec3(0, -100, 0);
				}	
			else
				{
					lifespan[i] -= 1;
				}

			if (positions[i].y >= -9.5)
			{
				//Step 1
				positions[i] = positions[i] + (glm::vec3(dt)*velocitys[i]) + glm::vec3(0.5f*(dt*dt))*accel;
				//Step 2
				velocitys[i] = velocitys[i] + (glm::vec3(0.5f*dt)*accel);

			}
			else
			{
				
				positions[i].y = -9.4;
				velocitys[i] = -velocitys[i] * (glm::vec3(0, 0.2, 0));

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

									  // Now draw the particles... as easy as this!
	//glPointSize(5);
	glDrawArrays(GL_POINTS, 0, NoP);
	glBindVertexArray(0);




}