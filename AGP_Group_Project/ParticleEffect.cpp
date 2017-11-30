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
	bounceValue = glm::vec3(0, 0.15, 0);
	iterator = 0;
	acceleration = glm::vec3(0, -80, 0);
	start = 0;
	lifespan = 500;
	bounceLifespan = 60;

	init();

	// Initialise all particles
	for (int i = 0; i < NoP; i++)
	{
		positions.push_back(emitPosition);
		velocitys.push_back(glm::vec3(0,0,0));
		colours.push_back(glm::vec4(0, 0, 1, 1));
		accel.push_back(glm::vec3(0, 0, 0));
		isAlive.push_back(false);
		age.push_back(lifespan);
		texID.push_back(particleTextures[0]);


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
}



float it = 0;
int k;
glm::vec3 launchVelocity;

void ParticleEffect::emitParticle(float speedMultiplier, float dt, bool velNotPos)
{
	// Iterate over and re-emit particles
	iterator = int(it);

	if (iterator < NoP)
	{
		if (velNotPos) {
			// Calculate
			float circleSegment = (360.0f / NoP);
			float angle = (circleSegment * iterator);
			float initialSpeed = (rand() % 111 / 20)  * speedMultiplier;
			if (initialSpeed < 1)
				initialSpeed++;
			launchVelocity = glm::vec3(0.0f + (initialSpeed * std::cos(angle)), 1.0f, 0.0f + (initialSpeed * std::sin(angle)));
			// Reset particle data
			positions[iterator] = emitPosition;
			velocitys[iterator] = glm::vec3(launchVelocity);
		}
		else
		{
			// Calculate...
			int multiplier = 1;
			if (iterator % 2 == 0){ multiplier = -1; }	
			float xOffset = rand() % 111 / 10 * multiplier;
			float zOffset = rand() % 111 / 10 * multiplier;
			glm::vec3 launchPos = glm::vec3(emitPosition.x + xOffset, emitPosition.y, emitPosition.z + zOffset);

			float circleSegment = (360.0f / NoP);
			float angle = (circleSegment * iterator);
			float initialSpeed = (rand() % 111 / 30);
			launchVelocity = glm::vec3(0.0f + (initialSpeed * std::cos(angle)), 1.0f, 0.0f + (initialSpeed * std::sin(angle)));

			// Reset particle data
			positions[iterator] = launchPos;
			velocitys[iterator] = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		accel[iterator] = acceleration;
		isAlive[iterator] = true;
		age[iterator] = lifespan;
		texID[iterator] = particleTextures[0];;
		colours[iterator] = glm::vec4(0.9f, 0.9f, 0.95f, 0.7f);

		it += 100*dt;
	}
	else
	{	//we've emitted all particles(NoP), start re-emitting from particle 0
		it = 0;
	}
		
}


void ParticleEffect::setEmitPos(glm::vec3 pos)
{
	this->emitPosition = pos;
}

void ParticleEffect::init(void)
{
	particleTextures[0] = TextureUtils::loadPNG("rainTex.png");
	particleTextures[1] = TextureUtils::loadPNG("rainTex2.png");
	particleTextures[2] = TextureUtils::loadPNG("rainTex3.png");
}

void ParticleEffect::update(float multiplier, bool isASprinkler)
{
	double dt = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	//glBindTexture(GL_TEXTURE_2D, particleTexture);


	// Initialise next particle
	emitParticle(multiplier, dt, isASprinkler);

	// Iterate over all particles
	for (int i = 0; i < NoP; i++)
	{
		// If particle has expired has reached end of life mark as dead or decrement lifespan
		if (age[i] <= 0)
		{
			isAlive[i] = false;
			positions[i] = glm::vec3(0.0f, -1000.0f, 0.0f);
		}
		else if(age[i] != 0 && isAlive[i])
		{
			age[i] -= dt;
		}
			
		// If the particle is alive keep it moving
		if (isAlive[i])
		{		
			positions[i] = positions[i] + (glm::vec3(dt)*velocitys[i]) + glm::vec3(0.5f*(dt*dt))*accel[i];
			velocitys[i] = velocitys[i] + (glm::vec3(0.5f*dt)*accel[i]);	
		}
			
		// Check the particle's speed...
		if (velocitys[i].y <= -9.0f)
		{
			texID[i] = particleTextures[0];
		}
		else if (velocitys[i].y >= -4.0f)
		{
			texID[i] = particleTextures[2];
		}
		else
		{
			texID[i] = particleTextures[1];
		}

			//for all possible collidables check for collision
			for (int j = 0; j < collidableObjects.size(); j++)
			{
				if (collisionTester->AABBtoPoint(collidableObjects[j]->getMin(), collidableObjects[j]->getMax(), positions[i]) == true)
				{
					//glm::vec3 normal = launchVelocity * 0.75f;
					//Addjusts height since the particle is probably partially inside the collideable object
					positions[i].y += 0.2;

					//reflect upwards and dampen velocity
					glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f); //Setup a default normal value (Assumes top surfaces only, no sides.)
					glm::vec3 reflectVector = velocitys[i] - (2*(glm::dot(velocitys[i], normal))*normal);

					float dampeningValue = 0.75f;
					velocitys[i] = glm::vec3(reflectVector.x * dampeningValue, reflectVector.y * (dampeningValue * 0.5), reflectVector.z * dampeningValue);

					//velocitys[i] = reflectVector * dampeningValue;

					//reflect in respect to the surface norm
					//velocitys[i] += normal;

					//Sets the lifespan so it will die shortly after bounce
					age[i] -= 250 * dt;
				
					//slightly increase alpha
					colours[i].a = 0.65f;				
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
	for (int i = 0; i < NoP; i++)
	{

		glBindTexture(GL_TEXTURE_2D, texID[i]);

		glDrawArrays(GL_POINTS, i, 1);
	}
	glBindVertexArray(0);




}