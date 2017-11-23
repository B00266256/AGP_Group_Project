#include "ParticleEffect.h"

ParticleEffect::ParticleEffect(int numOfParticles) : NoP(numOfParticles)
{
	//init values
	srand(time(0));
	accel = glm::vec3(0, -9.8, 0);

	float alpha = 0.0;

	for (int i = 0; i < NoP; i++)
	{
		positions.push_back(glm::vec3(-15, 2, -35));
		velocitys.push_back(glm::vec3(0, 10, 0));
		colours.push_back(glm::vec4(0, 0, 0, 1));

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
	
		//Verlet Solver
		
		for (int i = 0; i < NoP; i++)
		{

			if (positions[i].y >= -9.5)
			{
				//Step 1
				positions[i] = positions[i] + (glm::vec3(dt)*velocitys[i]) + glm::vec3(0.5f*(dt*dt))*accel;
				//Step 2
				velocitys[i] = velocitys[i] + (glm::vec3(0.5f*dt)*accel);
				//Step 4
				velocitys[i] = velocitys[i] + (glm::vec3(0.5f*dt)*accel);
			} 
			else
			{
				positions[i].y = -9.2;

				velocitys[i] = -velocitys[i] * glm::vec3(0.2);

				//positions[i] = positions[i] + (glm::vec3(dt)*velocitys[i]) + glm::vec3(0.5f*(dt*dt))*accel;

		
				
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