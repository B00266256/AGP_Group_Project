#include "ParticleEffect.h"

ParticleEffect::ParticleEffect(int numOfParticles) : NoP(numOfParticles)
{
	//init values
	for (int i = 0; i < positions.size(); i++)
	{
		positions[i] = glm::vec3(0, 0, -10);
		velocity[i] = glm::vec3(-0.1, 0, 0);
		colours[i] = glm::vec3(0, 0, 0);
	}
	//init vao/vbo

	glGenVertexArrays(1, vao);
	glGenBuffers(2, vbo);
	glBindVertexArray(vao[0]); // bind VAO 0 as current object

							   // Position data in attribute index 0, 3 floats per vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO for positions
	glBufferData(GL_ARRAY_BUFFER, NoP * 3 * sizeof(GLfloat), (GLfloat*)positions.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index 0

									  // Colours data in attribute 1, 3 floats per vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // bind VBO for colours
	glBufferData(GL_ARRAY_BUFFER, NoP * 3 * sizeof(GLfloat), (GLfloat*)colours.data(), GL_STATIC_DRAW);
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




void ParticleEffect::update()
{
	for (int i = 0; i < positions.size(); i++)
	{
		this->positions[i] += this->velocity[i];
	}

}





void ParticleEffect::draw()
{
	glBindVertexArray(vao[0]); // bind VAO 0 as current object

							   // particle data may have been updated - so need to resend to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO 0
	glBufferData(GL_ARRAY_BUFFER, NoP * 3 * sizeof(GLfloat), (GLfloat*)positions.data(), GL_DYNAMIC_DRAW);

	// Position data in attribute index 0, 3 floats per vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index 0

									  // Now draw the particles... as easy as this!
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, NoP);
	glBindVertexArray(0);




}