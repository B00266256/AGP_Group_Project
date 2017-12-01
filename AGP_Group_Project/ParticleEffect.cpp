#include "ParticleEffect.h"
#include <math.h>

#define DEG_TO_RADIAN 0.017453293

ParticleEffect::ParticleEffect()
{
	ParticleEffect(500, glm::vec3(0.0f, 0.0f, 0.0f));
}

ParticleEffect::ParticleEffect(int numOfParticles, glm::vec3 pos) : NoP(numOfParticles)
{
	//Initial values...
	srand(time(0));
	emitPosition = pos;
	start = 0;
	iterator = 0;
	accelerationConst = glm::vec3(0, -80, 0);
	lifespan = 500;
	particleColour = glm::vec4(0.9f, 0.9f, 0.95f, 0.5f);
	particlePointSize = 100;
}



ParticleEffect::~ParticleEffect()
{
}



void ParticleEffect::init(void)
{
	//Initialise our collision-checking class...
	collisionTester = new CollisionTest();
	//Initialise all of our textures...
	particleTextures[0] = TextureUtils::loadPNG("rainTex.png");
	particleTextures[1] = TextureUtils::loadPNG("rainTex2.png");
	particleTextures[2] = TextureUtils::loadPNG("rainTex3.png");
	//Initialise all of our particles:
	for (int i = 0; i < NoP; i++)
	{
		positions.push_back(emitPosition);			//positions at the emitposition.
		velocitys.push_back(glm::vec3(0, 0, 0));	//velocities as static.
		colours.push_back(glm::vec4(0, 0, 1, 1));	//colours as white.
		acceleration.push_back(glm::vec3(0, 0, 0));		//acceleration as static.
		age.push_back(lifespan);					//age as the default particle lifespan.
		texID.push_back(particleTextures[0]);		//texture as the default particle texture.
		isAlive.push_back(false);					//assume the particle is dead and will need to be emitted in update.
		sizes.push_back(particlePointSize);			//give the particle a default size.
	}
	//Initialise the VAO/VBO's...
	glGenVertexArrays(1, vao);
	glGenBuffers(2, vbo);
	glBindVertexArray(vao[0]); // bind VAO 0 as current object.

	// Position data in attribute index 0, 3 floats per vertex.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO for positions.
	glBufferData(GL_ARRAY_BUFFER, NoP * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index 0.

	// Colours data in attribute 1, 3 floats per vertex.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // bind VBO for colours.
	glBufferData(GL_ARRAY_BUFFER, NoP * sizeof(glm::vec4), colours.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);    // Enable attribute index 1.
	glBindVertexArray(0);
}



void ParticleEffect::update(float multiplier, bool isASprinkler)
{
	double dt = (std::clock() - start) / (double)CLOCKS_PER_SEC; //determine the delta-time by checking current time against the last update.
	emitParticle(multiplier, dt, isASprinkler);	//emit a particle.

	//For each particle in the system:
	for (int i = 0; i < NoP; i++)
	{
		// Check if a particle has reached end of its lifespan:
		if (age[i] > 0) 
		{
			age[i] -= dt; //If it has not: decrement the life of the particle by the time-difference since the previous update.

			//Update the velocity by the acceleration, and then the position by the velocity...
			velocitys[i] = velocitys[i] + (glm::vec3(dt)*acceleration[i]);
			positions[i] = positions[i] + (glm::vec3(dt)*velocitys[i]);

			//Set the particle texture based on the particle's downwards velocity...
			if (velocitys[i].y <= -9.0f) texID[i] = particleTextures[0];
			else if (velocitys[i].y >= -4.0f) texID[i] = particleTextures[2];
			else texID[i] = particleTextures[1];

			//Check each particle against each collidable object in the scene (Uses a brute force collision detection approach)...
			for (int j = 0; j < collidableObjects.size(); j++)
			{
				if (collisionTester->AABBtoPoint(collidableObjects[j]->getMin(), collidableObjects[j]->getMax(), positions[i]) == true)
				{
					//Adjusts particle position by a small positive Y value, since the particle is probably slightly inside the other object...
					positions[i].y += 0.2f;
					//Calculate the reflection-vector and detemine a dampening value...
					glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f); //Setup a default normal value (Assumes top surfaces only, no sides.)
					glm::vec3 reflectVector = velocitys[i] - (2*(glm::dot(velocitys[i], normal))*normal);
					float dampeningValue = 0.33f;
					//Sets the collided particle's velocity to the reflect-vector and applies a dampening value, should produce realistic collision reaction...
					velocitys[i] = glm::vec3(reflectVector.x, reflectVector.y * dampeningValue, reflectVector.z);
					//Decrements the lifespan significantly to kill off particles after their first collisions...
					age[i] -= 250;		
				}
			}
		} else {
			//If the particle HAS reached the end of it's lifespan:
			isAlive[i] = false; //consider the particle to be dead.
			positions[i] = glm::vec3(0.0f, -1000.0f, 0.0f); //move it offscreen. (It will be re-emitted later).
		}
	}
	start = std::clock(); //Set the start value (this will be used next update to determine the delta-time).
	//End of method.
}



void ParticleEffect::draw()
{
	//Update the positions...
	glBindVertexArray(vao[0]); // bind VAO 0 as current object.
	// particle data may have been updated - so need to resend to GPU.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO 0.
	glBufferData(GL_ARRAY_BUFFER, NoP * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
	// Position data in attribute index 0, 3 floats per vertex.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index.

	//Update the colours...
	glBindVertexArray(vao[1]); // bind VAO 0 as current object.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // bind VBO 0.
	glBufferData(GL_ARRAY_BUFFER, NoP * sizeof(glm::vec4), colours.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index.

	//Update the sizes...
	glBindVertexArray(vao[2]); // bind VAO 0 as current object.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // bind VBO 0.
	glBufferData(GL_ARRAY_BUFFER, NoP * sizeof(int), sizes.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);     // Enable attribute index.

	//Draw all of our particles...
	for (int i = 0; i < NoP; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glDrawArrays(GL_POINTS, i, 1);
	}
	glBindVertexArray(0);
}


//Emmision globals...
float it = 0;
int k;
glm::vec3 launchVelocity;

void ParticleEffect::emitParticle(float speedMultiplier, float dt, bool isASprinkler)
{
	//"it" and "iterator" are used to iterate through the list of particles, "it" is cast to an integer after being updated by the delta time, ->
	// -> this should tie our particle emmisions to the clock rather than the framerate...
	iterator = int(it);

	if (iterator < NoP) 
	{
		//Creates a particle with outwards-circular-velocity from the emit-position (A sprinkler!)...
		if (isASprinkler) {
			//Calculate the launch vector based on the number of particles assuming they are launched sequentially from the center of a circle...
			float circleSegment = (360.0f / NoP);
			float angle = (circleSegment * iterator);
			//Launch speed is determined based on a capped random value multiplied by our user-controlled speed multiplier...
			float initialSpeed = (rand() % 111 / 20)  * speedMultiplier;
			//Assume speed cannot be negative, caps user input...
			if (initialSpeed < 1) initialSpeed = 0.1f;
			//Launch vector is defined using the pre-determined speed and angle at which it is launched from the center of the circle...
			launchVelocity = glm::vec3(0.0f + (initialSpeed * std::cos(angle)), 1.0f, 0.0f + (initialSpeed * std::sin(angle)));
			//Set the emmited particle data...
			positions[iterator] = emitPosition;
			velocitys[iterator] = glm::vec3(launchVelocity);
		}
		//Otherwise simulate a rainfall effect, similar, but with randomised particle emit positions and less launch velocity...
		else
		{
			//Multiplier is used to determine wether the particle position is in the negative or positive...
			int multiplier = 1; 
			if (iterator % 2 == 0) { multiplier = -1; }
			//In this case, position is randomised as a capped offset from the emit-position...
			float xOffset = rand() % 111 / 10 * multiplier;
			float zOffset = rand() % 111 / 10 * multiplier;
			glm::vec3 launchPos = glm::vec3(emitPosition.x + xOffset, emitPosition.y, emitPosition.z + zOffset);
			//Launch vector is defined much the same as above, although without the user-control...
			float circleSegment = (360.0f / NoP);
			float angle = (circleSegment * iterator);
			float initialSpeed = (rand() % 111 / 30);
			launchVelocity = glm::vec3(0.0f + (initialSpeed * std::cos(angle)), 1.0f, 0.0f + (initialSpeed * std::sin(angle)));
			//Set the emmited particle data...
			positions[iterator] = launchPos;
			velocitys[iterator] = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		//Setup the particle with all other relevant values:
		acceleration[iterator] = accelerationConst;
		isAlive[iterator] = true;	//set the particle as alive so that it will update.
		age[iterator] = lifespan;	//reset the particle to the default lifespan.
		texID[iterator] = particleTextures[0]; //set a default texture.
		colours[iterator] = particleColour;	//set an initial colour.
		sizes[iterator] = particlePointSize; //set an initial size.

		//Update the iteration vaules by a factor of the delta-time...
		it += 100 * dt;
	}
	else
	{	//All of our particles have been emmited for this iteration, start re-emitting from particle 0...
		it = 0;
	}

}
