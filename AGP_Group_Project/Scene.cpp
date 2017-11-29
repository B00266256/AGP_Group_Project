#include "Scene.h"

glm::vec3 sprinklerPos = glm::vec3(0.0f, 0.0f, 0.0f);
float particleLaunchMultiplier = 1.0f;
bool sprinklerActive = true;
bool tetherSprinkler = true;
bool isASprinkler = false;

Scene::Scene()
{
	//model view and camera setup
	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 800.0f / 600.0f, 0.5f, 2000.0f);
	glm::mat4 modelview(1.0);
	mvStack.push(modelview);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	eye = glm::vec3(0.0f, 3.0f, -5.0f);
	at = glm::vec3(0.0f, 1.0f, -1.0f);
	mvStack.top() = glm::lookAt(eye, at, up);

	renderer = new Renderer();

	// light attenuation
	attConstant = 1.0f;
	attLinear = 0.05f;
	attQuadratic = 0.01f;

	//define the spotlights cone		
	angle1 = 15;																						
	angle2 = 20;

	spotLightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	spotLightPos = glm::vec4(-15.0f, 5.0f, -20, 1.0f);

	//Phong tex shader program
	shaderID[0] = shader.init("phong-tex.vert", "phong-tex.frag");
	shader.setLight(shaderID[0], light);
	shader.setMaterial(shaderID[0], material);
	shader.setProjection(shaderID[0], projection);
	shader.setMVP(shaderID[0], mvStack.top());
	shader.setSpotlight(shaderID[0], spotLight, glm::value_ptr(spotLightDirection));

	//spotlight angle updates
	GLuint uniformIndex = glGetUniformLocation(shaderID[0], "angle1");
	glUniform1i(uniformIndex, angle1);
	uniformIndex = glGetUniformLocation(shaderID[0], "angle2");
	glUniform1i(uniformIndex, angle2);

	uniformIndex = glGetUniformLocation(shaderID[0], "attConst");
	glUniform1f(uniformIndex, attConstant);
	uniformIndex = glGetUniformLocation(shaderID[0], "attLinear");
	glUniform1f(uniformIndex, attLinear);
	uniformIndex = glGetUniformLocation(shaderID[0], "attQuadratic");
	glUniform1f(uniformIndex, attQuadratic);

	//Particle Shader
	shaderID[1] = shader.init("Particle.vert", "Particle.frag");
	shader.setProjection(shaderID[1], projection);
	shader.setMVP(shaderID[1], mvStack.top());
	shader.setSpotlight(shaderID[1], spotLight, glm::value_ptr(spotLightDirection));

	//spotlight angle updates
	uniformIndex = glGetUniformLocation(shaderID[1], "angle1");
	glUniform1i(uniformIndex, angle1);
	uniformIndex = glGetUniformLocation(shaderID[1], "angle2");
	glUniform1i(uniformIndex, angle2);

	uniformIndex = glGetUniformLocation(shaderID[1], "attConst");
	glUniform1f(uniformIndex, attConstant);
	uniformIndex = glGetUniformLocation(shaderID[1], "attLinear");
	glUniform1f(uniformIndex, attLinear);
	uniformIndex = glGetUniformLocation(shaderID[1], "attQuadratic");
	glUniform1f(uniformIndex, attQuadratic);

	//Normal map shader program
	shaderID[0] = shader.init("normalmap.vert", "normalmap.frag");
	shader.setLight(shaderID[0], light);
	shader.setMaterial(shaderID[0], material);
	shader.setProjection(shaderID[0], projection);
	shader.setMVP(shaderID[0], mvStack.top());
	shader.setSpotlight(shaderID[0], spotLight, glm::value_ptr(spotLightDirection));

	//spotlight angle updates
	uniformIndex = glGetUniformLocation(shaderID[0], "angle1");
	glUniform1i(uniformIndex, angle1);
	uniformIndex = glGetUniformLocation(shaderID[0], "angle2");
	glUniform1i(uniformIndex, angle2);

	uniformIndex = glGetUniformLocation(shaderID[0], "attConst");
	glUniform1f(uniformIndex, attConstant);
	uniformIndex = glGetUniformLocation(shaderID[0], "attLinear");
	glUniform1f(uniformIndex, attLinear);
	uniformIndex = glGetUniformLocation(shaderID[0], "attQuadratic");
	glUniform1f(uniformIndex, attQuadratic);

	//init initialises all game objects and pushes them all into a vector for passing to the renderer.
	init();
}



void Scene::init()
{
	

	//particle effect
	glEnable(GL_POINT_SPRITE);

	//Allows point size to be determined wihtin the vertex shader
	glEnable(GL_PROGRAM_POINT_SIZE);

	Sprinkler = new ParticleEffect(500, glm::vec3(-15, 20, -30));
	particleTexture = TextureUtils::loadBitmap("rain.bmp");

	//Creates and pushes all the objects representing the building
	loadScene();
}

glm::vec3 Scene::moveForward(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::sin(r*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(r*DEG_TO_RADIAN));
}

glm::vec3 Scene::moveRight(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::cos(r*DEG_TO_RADIAN), pos.y, pos.z + d*std::sin(r*DEG_TO_RADIAN));
}



//send out pile of game object to the renderer along with the model view stack

void Scene::draw()
{
	// clear the screen
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	
	//Game Objects
	renderer->draw(gameObjects, mvStack);

<<<<<<< HEAD

	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-10, 3, -10));
	shader.setMVP(shaderID[0], mvStack.top());
	shader.setSpotlight(shaderID[0], spotLight, glm::value_ptr(spotLightDirection));
	mvStack.pop();

=======
>>>>>>> refs/remotes/origin/master
	//update and draw particles
	glUseProgram(shaderID[1]);
	glBindTexture(GL_TEXTURE_2D, particleTexture);
	//update particle shader
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0, 0, 0));
	shader.setProjection(shaderID[1], projection);
	shader.setMVP(shaderID[1], mvStack.top());

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(0);

	Sprinkler->update(particleLaunchMultiplier, isASprinkler);
	//draw
	if (sprinklerActive) Sprinkler->draw();

	glDisable(GL_BLEND);
	glDepthMask(1);
	glDepthMask(GL_TRUE);

	mvStack.pop();
}

//Update function...
void Scene::update()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	//Update camera pos...
	if (keys[SDL_SCANCODE_W]) eye = moveForward(eye, r, 0.2f);
	if (keys[SDL_SCANCODE_S]) eye = moveForward(eye, r, -0.2f);
	if (keys[SDL_SCANCODE_A]) eye = moveRight(eye, r, -0.2f);
	if (keys[SDL_SCANCODE_D]) eye = moveRight(eye, r, 0.2f);
	if (keys[SDL_SCANCODE_R]) eye.y += 0.1f;
	if (keys[SDL_SCANCODE_F]) eye.y -= 0.1f;
	//Update camera rot...
	if (keys[SDL_SCANCODE_COMMA]) r -= 0.5f;
	if (keys[SDL_SCANCODE_PERIOD]) r += 0.5f;
	//Finalise camera values...
	at = moveForward(eye, r, 1.0f);
	mvStack.top() = glm::lookAt(eye, at, up);

	//Change draw mode...
	if (keys[SDL_SCANCODE_1]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	if (keys[SDL_SCANCODE_2]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	//Particle emitter controls...
	if (keys[SDL_SCANCODE_7]) {
		sprinklerActive = true;
		std::cout << "Particle emitter on!" << std::endl;
	}
	if (keys[SDL_SCANCODE_8]) {
		sprinklerActive = false;
		std::cout << "Particle emitter off!" << std::endl;
	}
	if (keys[SDL_SCANCODE_9]) {
		tetherSprinkler = true;
		isASprinkler = false;
		std::cout << "Particle emitter 'Sprinkler' tethered to camera (Rain Effect)" << std::endl;
	}
	if (keys[SDL_SCANCODE_0]) {
		tetherSprinkler = false;
		isASprinkler = true;
		std::cout << "Particle emitter 'Sprinkler' untethered from camera. Use arrow keys to move!" << std::endl;
	}
	if (keys[SDL_SCANCODE_MINUS]) {
		particleLaunchMultiplier -= 0.01;
	}
	if (keys[SDL_SCANCODE_EQUALS]) {
		particleLaunchMultiplier += 0.01;
	}

	if (tetherSprinkler)
	{
		sprinklerPos = moveForward(eye, r, 6.0f);
		sprinklerPos.y += 10.0f;
	}
	else {
		if (keys[SDL_SCANCODE_UP]) sprinklerPos = moveForward(sprinklerPos, r, 0.1f);
		if (keys[SDL_SCANCODE_DOWN]) sprinklerPos = moveForward(sprinklerPos, r, -0.1f);
		if (keys[SDL_SCANCODE_LEFT]) sprinklerPos = moveRight(sprinklerPos, r, -0.1f);
		if (keys[SDL_SCANCODE_RIGHT]) sprinklerPos = moveRight(sprinklerPos, r, 0.1f);
		if (keys[SDL_SCANCODE_N]) sprinklerPos.y += 0.1f;
		if (keys[SDL_SCANCODE_M]) sprinklerPos.y -= 0.1f;
	}
		Sprinkler->setEmitPos(glm::vec3(sprinklerPos.x, sprinklerPos.y, sprinklerPos.z));
}


void Scene::loadScene()
{


	//Collidable crates...
	//No.1
	GameObject *collidableBox = new GameObject(glm::vec3(5.0f, 0.0f, -5.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
	collidableBox->setTexture("crateTex.bmp");
	collidableBox->setMaterial(material);
	collidableBox->setMesh("cube.obj");
	collidableBox->setShader(shaderID[0]);
	collidableBox->addObjectInstance(glm::vec3(5.0f, 0.0f, -5.0f));
	gameObjects.push_back(collidableBox);
	//No.2
	GameObject *collidableBox2 = new GameObject(glm::vec3(18.0f, 0.0f, -14.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
	collidableBox2->setTexture("crateTex.bmp");
	collidableBox2->setMaterial(material);
	collidableBox2->setMesh("cube.obj");
	collidableBox2->setShader(shaderID[0]);
	collidableBox2->addObjectInstance(glm::vec3(18.0f, 0.0f, -14.0f));
	gameObjects.push_back(collidableBox2);
	//No.3
	GameObject *collidableBox3 = new GameObject(glm::vec3(14.0f, 0.0f, -18.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
	collidableBox3->setTexture("crateTex.bmp");
	collidableBox3->setMaterial(material);
	collidableBox3->setMesh("cube.obj");
	collidableBox3->setShader(shaderID[0]);
	collidableBox3->addObjectInstance(glm::vec3(14.0f, 0.0f, -18.0f));
	gameObjects.push_back(collidableBox3);
	//No.4
	GameObject *collidableBox4 = new GameObject(glm::vec3(18.0f, 4.0f, -18.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
	collidableBox4->setTexture("crateTex.bmp");
	collidableBox4->setMaterial(material);
	collidableBox4->setMesh("cube.obj");
	collidableBox4->setShader(shaderID[0]);
	collidableBox4->addObjectInstance(glm::vec3(18.0f, 4.0f, -18.0f));
	gameObjects.push_back(collidableBox4);
	//No.5
	GameObject *collidableBox5 = new GameObject(glm::vec3(10.0f, 0.0f, -10.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
	collidableBox5->setTexture("crateTex.bmp");
	collidableBox5->setMaterial(material);
	collidableBox5->setMesh("cube.obj");
	collidableBox5->setShader(shaderID[0]);
	collidableBox5->addObjectInstance(glm::vec3(10.0f, 0.0f, -10.0f));
	gameObjects.push_back(collidableBox5);
	//No.6
	GameObject *collidableBox6 = new GameObject(glm::vec3(-18.0f, 2.0f, -18.0f), glm::vec3(2, 4, 2), glm::vec3(NULL, NULL, NULL));
	collidableBox6->setTexture("crateTex.bmp");
	collidableBox6->setMaterial(material);
	collidableBox6->setMesh("cube.obj");
	collidableBox6->setShader(shaderID[0]);
	collidableBox6->addObjectInstance(glm::vec3(-18.0f, 2.0f, -18.0f));
	gameObjects.push_back(collidableBox5);


	// COLLISIONS
	// Initialise the bounding box and make this object something the particle effect can test for colliion with
	collidableBox->initAABB();
	collidableBox2->initAABB();
	collidableBox3->initAABB();
	collidableBox4->initAABB();
	collidableBox5->initAABB();
	collidableBox6->initAABB();
	Sprinkler->addtCollidableObjects(collidableBox);
	Sprinkler->addtCollidableObjects(collidableBox2);
	Sprinkler->addtCollidableObjects(collidableBox3);
	Sprinkler->addtCollidableObjects(collidableBox4);
	Sprinkler->addtCollidableObjects(collidableBox5);
	Sprinkler->addtCollidableObjects(collidableBox6);




	//Ground Planes
	GameObject *groundPlane = new GameObject(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(20, 0.1, 20), glm::vec3(NULL, NULL, NULL));
	groundPlane->setTexture("concrete.bmp");
	groundPlane->setMaterial(material);
	groundPlane->setMesh("cube.obj");
	groundPlane->setShader(shaderID[0]);
	groundPlane->addObjectInstance(glm::vec3(0.0f, -2.0f, 0.0f));
	gameObjects.push_back(groundPlane);

	// COLLISIONS
	// Initialise the bounding box and make this object something the particle effect can test for colliion with
	groundPlane->initAABB();
	Sprinkler->addtCollidableObjects(groundPlane);



	//Roof Planes
	GameObject *roofPlane = new GameObject(glm::vec3(0.0f, 13.0f, 0.0f), glm::vec3(20, 0.1, 20), glm::vec3(NULL, NULL, NULL));
	roofPlane->setTexture("concrete.bmp");
	roofPlane->setMaterial(material);
	roofPlane->setMesh("cube.obj");
	roofPlane->setShader(shaderID[0]);
	roofPlane->addObjectInstance(glm::vec3(0.0f, 13.0f, 0.0f));
	gameObjects.push_back(roofPlane);
	gameObjects.push_back(roofPlane);

	
	//Left Wall Planes
	GameObject *wall = new GameObject(glm::vec3(-20.1f, 5.5f, 0.0f), glm::vec3(0.1f, 7.5f, 10.0f), glm::vec3(NULL, NULL, NULL));
	wall->setTexture("brickwall.bmp");
	wall->setMaterial(material);
	wall->setMesh("cube.obj");
	wall->setShader(shaderID[0]);
	wall->addObjectInstance(glm::vec3(-20.1f, 5.5f, 10.0f));
	wall->addObjectInstance(glm::vec3(-20.1f, 5.5f, -10.0f));
	gameObjects.push_back(wall);

	//Right Wall Planes
	GameObject *wall2 = new GameObject(glm::vec3(20.1f, 5.5f, 0.0f), glm::vec3(0.1f, 7.5f, 10.0f), glm::vec3(NULL, 1.0f, NULL), 180.0f);
	wall2->setTexture("brickwall.bmp");
	wall2->setMaterial(material);
	wall2->setMesh("cube.obj");
	wall2->setShader(shaderID[0]);
	wall2->addObjectInstance(glm::vec3(20.1f, 5.5f, 10.0f));
	wall2->addObjectInstance(glm::vec3(20.1f, 5.5f, -10.0f));
	gameObjects.push_back(wall2);

	//Front Wall Planes
	GameObject *wall3 = new GameObject(glm::vec3(0.0f, 5.5f, -20.1f), glm::vec3(10.0f, 7.5f, 0.1f), glm::vec3(NULL, NULL, NULL));
	wall3->setTexture("brickwall.bmp");
	wall3->setMaterial(material);
	wall3->setMesh("cube.obj");
	wall3->setShader(shaderID[0]);
	wall3->addObjectInstance(glm::vec3(10.0f, 5.5f, -20.1f));
	wall3->addObjectInstance(glm::vec3(-10.0f, 5.5f, -20.1f));
	gameObjects.push_back(wall3);

	//Rear Wall Planes
	GameObject *wall4 = new GameObject(glm::vec3(0.0f, 5.5f, 20.1f), glm::vec3(10.0f, 7.5f, 0.1f), glm::vec3(NULL, NULL, NULL));
	wall4->setTexture("brickwall.bmp");
	wall4->setMaterial(material);
	wall4->setMesh("cube.obj");
	wall4->setShader(shaderID[0]);
	wall4->addObjectInstance(glm::vec3(10.0f, 5.5f, 20.1f));
	wall4->addObjectInstance(glm::vec3(-10.0f, 5.5f, 20.1f));
	gameObjects.push_back(wall4);

}