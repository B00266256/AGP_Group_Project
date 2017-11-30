//ParticleEffect.h
//AGP Group Project B00266256, B00Chloe
//"Used to define our falling water particle system."

#include <glm/glm.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "rt3d.h"
#include "collisionTest.h"
#include "GameObject.h"

class ParticleEffect
{
public:

	ParticleEffect();						//Constructor - Default.
	ParticleEffect(int n, glm::vec3 pos);	//Constructor.
	~ParticleEffect();						//Destructor.

	//Default methods...
	void init(void);									//Initialises the particles and particle system.
	void update(float multiplier, bool isASprinkler);	//Updates the particles within our system (applies physics, updates positions and other values.)
	void draw(void);									//Renders the particles to the scene.


	void emitParticle(float speedMultiplier, float dt, bool isASprinkler);	//emits a new particle. (Pass in mutiplier for launch velocity, delta-time of the update and if we want a sprinkler effect.
	void addtCollidableObjects(GameObject * object) { collidableObjects.push_back(object); }	//adds a new collidable object to the list.
	void setEmitPos(glm::vec3 pos) { emitPosition = pos; }										//set method for emit position.
	void setParticleColour(glm::vec4 colour) { particleColour = colour; }						//set method for emit colour.

	//Get methods for private variables...
	int getNoP() { return NoP; }

	std::vector<glm::vec3> getPositions() const { return positions; }
	std::vector<glm::vec4> getColours() const { return colours; }
	std::vector<glm::vec3> getVel() const { return velocitys; }
	std::vector<glm::vec3> getAcceleration() const { return acceleration; }
	std::vector<GLuint> getTexID() const { return texID; }
	std::vector<float> getAge() const { return age; }
	std::vector<bool> getIsAlive() const{ return isAlive; }

private:
	CollisionTest *collisionTester; //collision testing class for use in particle collision.
	std::vector<GameObject*> collidableObjects; //vector list of all collidable objects in the scene.

	//Particle values - Each value applies to a single iteration of each particle's:
	std::vector<glm::vec3> positions;		//current position.
	std::vector<glm::vec3> velocitys;		//current velocity.
	std::vector<glm::vec3> acceleration;	//current acceleration.
	std::vector<glm::vec4> colours;			//current colour.
	std::vector<GLuint> texID;				//current texture.
	std::vector<float> age;					//current lifespan remaining.
	std::vector<bool> isAlive;				//current status with regards to its use.



	glm::vec3 emitPosition;		//launch position for particles when created.
	glm::vec3 accelerationConst;//constant value to define a particles downwards acceleration.
	glm::vec4 particleColour;	//colour of newly-emitted particles (allows user to change them).
	GLuint particleTextures[3]; //list of possible textures used by particles (used for dynamicly-animated particles, altering textures under different conditions)
	int lifespan;				//the default value for how long our particles last before being removed from the scene.
	int NoP;					//the number of particles defined in the scene, iterates through them when emitting.
	int iterator;				//used to iterate through all particles when emitting them.

	//Used in locking our particle effects to a clock rather than the framerate...
	std::clock_t start;
	double dt;

	//VAO & VBO's
	GLuint vao[1];
	GLuint vbo[2];
};
