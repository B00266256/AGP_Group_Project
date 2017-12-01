#include "Scene.h"
#include "collisionTest.h"

GameObject *collectable;
CollisionTest *col;
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
	eye = glm::vec3(-14.0f, 2.5f, 14.0f);
	at = glm::vec3(1.0f, 0.0f, -1.0f);
	mvStack.top() = glm::lookAt(eye, at, up);

	renderer = new Renderer();

	// light attenuation
	attConstant = 1.0f;
	attLinear = 0.05f;
	attQuadratic = 0.01f;

	//define the spotlights cone		
	angle1 = 20;																						
	angle2 = 25;

	spotLightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	spotLightPos = glm::vec4(-15.0f, 5.0f, -20, 1.0f);

	//Phong tex shader program
	shaderID[0] = shader.init("phong-tex.vert", "phong-tex.frag");
	shader.setLight(shaderID[0], light);
	shader.setMaterial(shaderID[0], material);
	shader.setProjection(shaderID[0], projection);
	shader.setMVP(shaderID[0], mvStack.top());
	shader.setSpotlight(shaderID[0], spotLight, glm::value_ptr(spotLightDirection));
	shader.setAttenuation(shaderID[0], attConstant, attLinear, attConstant);
	shader.setSpotlightAngles(shaderID[0], angle1, angle2);



	//Particle Shader...
	shaderID[1] = shader.init("Particle.vert", "Particle.frag");
	shader.setProjection(shaderID[1], projection);
	shader.setMVP(shaderID[1], mvStack.top());
	shader.setSpotlight(shaderID[1], spotLight, glm::value_ptr(spotLightDirection));
	shader.setAttenuation(shaderID[1], attConstant, attLinear, attConstant);
	shader.setSpotlightAngles(shaderID[1], angle1, angle2);


	//Normal map shader program...
	shaderID[2] = shader.init("normalmap.vert", "normalmap.frag");
	shader.setLight(shaderID[2], light);
	shader.setMaterial(shaderID[2], material);
	shader.setProjection(shaderID[2], projection);
	shader.setMVP(shaderID[2], mvStack.top());
	shader.setSpotlight(shaderID[2], spotLight, glm::value_ptr(spotLightDirection));

	shader.setSpotlightAngles(shaderID[2], angle1, angle2);
	shader.setAttenuation(shaderID[2], attConstant, attLinear, attConstant);

	//init initialises all game objects and pushes them all into a vector for passing to the renderer.
	init();
}



void Scene::init()
{
	
	//particle effect
	glEnable(GL_POINT_SPRITE);

	//Allows point size to be determined wihtin the vertex shader. This took a bit of research to do.
	//In the original labs particles got bigger as you moved away giving a really unrealistic effect. With this, the shader
	//can determine the size based on the distance between the camera and the particle keeping the size constant.
	glEnable(GL_PROGRAM_POINT_SIZE);

	//Particle Effect (Sprinkler/Rain)
	Sprinkler = new ParticleEffect(100, glm::vec3(-15, 20, -30));
	Sprinkler->init();
	//Creates and pushes all the objects representing the building.
	loadScene();

	col = new CollisionTest();
}

glm::vec3 Scene::moveForward(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::sin(r*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(r*DEG_TO_RADIAN));
}

glm::vec3 Scene::moveRight(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::cos(r*DEG_TO_RADIAN), pos.y, pos.z + d*std::sin(r*DEG_TO_RADIAN));
}



//send out pile of game object to the renderer along with the model view stack which has update to date camera data
void Scene::draw()
{
	// clear the screen
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	//draw Game Objects (scene)
	renderer->draw(gameObjects, mvStack);

	//PARTICLES

	//update and draw particles
	glUseProgram(shaderID[1]);
	glBindTexture(GL_TEXTURE_2D, particleTexture);

	//Update particle shader
	shader.setProjection(shaderID[1], projection);
	shader.setMVP(shaderID[1], mvStack.top());


	//draw particles
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(0);

	Sprinkler->update(particleLaunchMultiplier, isASprinkler);
	
	if (sprinklerActive) Sprinkler->draw();

	glDisable(GL_BLEND);
	glDepthMask(1);
	glDepthMask(GL_TRUE);

}

//Update function for input and updating based on new inputs
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
		std::cout << "Particle emitter tethered to camera. Rain Effect!" << std::endl;
	}
	if (keys[SDL_SCANCODE_0]) {
		tetherSprinkler = false;
		isASprinkler = true;
		std::cout << "Particle emitter untethered from camera. Use arrow keys to move!" << std::endl;
	}
	if (keys[SDL_SCANCODE_MINUS]) {
		particleLaunchMultiplier -= 0.05f;
	}
	if (keys[SDL_SCANCODE_EQUALS]) {
		particleLaunchMultiplier += 0.05f;
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

	if (keys[SDL_SCANCODE_KP_1]) { Sprinkler->setParticleColour(glm::vec4(1.0f, 0.0f, 0.0f, 0.9f)); }
	if (keys[SDL_SCANCODE_KP_2]) { Sprinkler->setParticleColour(glm::vec4(0.0f, 1.0f, 0.0f, 0.9f)); }
	if (keys[SDL_SCANCODE_KP_3]) { Sprinkler->setParticleColour(glm::vec4(0.0f, 0.0f, 1.0f, 0.9f)); }
	if (keys[SDL_SCANCODE_KP_5]) { Sprinkler->setParticleColour(glm::vec4(0.8f, 0.8f, 0.8f, 0.6f)); }

	//spotlight controls - IJKL - changle angle of outer and inner cones of spotlight
	if (keys[SDL_SCANCODE_I])
	{
		if (angle2 < 50) angle2 += 1;													//These if's cap the angles used for the spotlight. 
	}
	if (keys[SDL_SCANCODE_J])
	{
		if (angle1 > 0) angle1 -= 1; 
	}
	if (keys[SDL_SCANCODE_K])
	{
		if (angle2 > 20) angle2 -= 1; 
	}
	if (keys[SDL_SCANCODE_L])
	{
		if (angle1 < 20) angle1 += 1;  		
	}
	//Toggle Spotlight on and off
	if (keys[SDL_SCANCODE_O]) //use spotlight
	{
		spotLightDirection = glm::vec3(0, 0, 0);
	}
	if (keys[SDL_SCANCODE_P]) //use positional light
	{
		spotLightDirection = glm::vec3(0, 0, -1);
	}
	
	//update spotlight in shaders
	glUseProgram(shaderID[0]);
	shader.setSpotlightAngles(shaderID[0], angle1, angle2);
	shader.setSpotlight(shaderID[0], spotLight, glm::value_ptr(spotLightDirection));
		
	glUseProgram(shaderID[1]);
	shader.setSpotlightAngles(shaderID[1], angle1, angle2);
	shader.setSpotlight(shaderID[1], spotLight, glm::value_ptr(spotLightDirection));
		
	if (col->checkCollectableCollision(eye, collectable->getPos(0)))
	{
		std::cout << "Secret Documents Found!!! Press 'E' to quit as a winner!" << std::endl;
		if (keys[SDL_SCANCODE_E])
		{
			exit(5);
		}
	}

}


void Scene::loadScene()
{
	//Collidable crates...
	//First crate at origin...
		//No.1
		GameObject *collidableBox = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox->setTexture("crateTex.bmp");
		collidableBox->setMaterial(material);
		collidableBox->setMesh("cube.obj");
		collidableBox->setShader(shaderID[0]);
		collidableBox->addObjectInstance(glm::vec3(0.0f, 0.0f, 0.0f));
		gameObjects.push_back(collidableBox);
	//Rear Right Crates - x3 Stacked...
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
	//Rear Left Crates - x4 Stacked...
		//No.5
		GameObject *collidableBox5 = new GameObject(glm::vec3(-18.0f, 0.0f, -18.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox5->setTexture("crateTex.bmp");
		collidableBox5->setMaterial(material);
		collidableBox5->setMesh("cube.obj");
		collidableBox5->setShader(shaderID[0]);
		collidableBox5->addObjectInstance(glm::vec3(-18.0f, 0.0f, -18.0f));
		gameObjects.push_back(collidableBox5);
		//No.6
		GameObject *collidableBox6 = new GameObject(glm::vec3(-18.0f, 4.0f, -18.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox6->setTexture("crateTex.bmp");
		collidableBox6->setMaterial(material);
		collidableBox6->setMesh("cube.obj");
		collidableBox6->setShader(shaderID[0]);
		collidableBox6->addObjectInstance(glm::vec3(-18.0f, 4.0f, -18.0f));
		gameObjects.push_back(collidableBox6);
		//No.7
		GameObject *collidableBox7 = new GameObject(glm::vec3(-14.0f, 0.0f, -18.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox7->setTexture("crateTex.bmp");
		collidableBox7->setMaterial(material);
		collidableBox7->setMesh("cube.obj");
		collidableBox7->setShader(shaderID[0]);
		collidableBox7->addObjectInstance(glm::vec3(-14.0f, 0.0f, -18.0f));
		gameObjects.push_back(collidableBox7);
		//No.8
		GameObject *collidableBox8 = new GameObject(glm::vec3(-10.0f, 0.0f, -18.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox8->setTexture("crateTex.bmp");
		collidableBox8->setMaterial(material);
		collidableBox8->setMesh("cube.obj");
		collidableBox8->setShader(shaderID[0]);
		collidableBox8->addObjectInstance(glm::vec3(-10.0f, 0.0f, -18.0f));
		gameObjects.push_back(collidableBox8);
	//Center Left Crates - x3 On Floor...
		//No.9
		GameObject *collidableBox9 = new GameObject(glm::vec3(-18.0f, 0.0f, -4.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox9->setTexture("crateTex.bmp");
		collidableBox9->setMaterial(material);
		collidableBox9->setMesh("cube.obj");
		collidableBox9->setShader(shaderID[0]);
		collidableBox9->addObjectInstance(glm::vec3(-18.0f, 0.0f, -4.0f));
		gameObjects.push_back(collidableBox9);
		//No.10
		GameObject *collidableBox10 = new GameObject(glm::vec3(-18.0f, 0.0f, 0.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox10->setTexture("crateTex.bmp");
		collidableBox10->setMaterial(material);
		collidableBox10->setMesh("cube.obj");
		collidableBox10->setShader(shaderID[0]);
		collidableBox10->addObjectInstance(glm::vec3(-18.0f, 0.0f, 0.0f));
		gameObjects.push_back(collidableBox10);
		//No.11
		GameObject *collidableBox11 = new GameObject(glm::vec3(-14.0f, 0.0f, 0.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox11->setTexture("crateTex.bmp");
		collidableBox11->setMaterial(material);
		collidableBox11->setMesh("cube.obj");
		collidableBox11->setShader(shaderID[0]);
		collidableBox11->addObjectInstance(glm::vec3(-14.0f, 0.0f, 0.0f));
		gameObjects.push_back(collidableBox11);
	//Center Right Crates - x3 Stacked...
		//No.12
		GameObject *collidableBox12 = new GameObject(glm::vec3(18.0f, 0.0f, 4.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox12->setTexture("crateTex.bmp");
		collidableBox12->setMaterial(material);
		collidableBox12->setMesh("cube.obj");
		collidableBox12->setShader(shaderID[0]);
		collidableBox12->addObjectInstance(glm::vec3(18.0f, 0.0f, 4.0f));
		gameObjects.push_back(collidableBox12);
		//No.13
		GameObject *collidableBox13 = new GameObject(glm::vec3(18.0f, 0.0f, 0.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox13->setTexture("crateTex.bmp");
		collidableBox13->setMaterial(material);
		collidableBox13->setMesh("cube.obj");
		collidableBox13->setShader(shaderID[0]);
		collidableBox13->addObjectInstance(glm::vec3(18.0f, 0.0f, 0.0f));
		gameObjects.push_back(collidableBox13);
		//No.14
		GameObject *collidableBox14 = new GameObject(glm::vec3(18.0f, 4.0f, 4.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox14->setTexture("crateTex.bmp");
		collidableBox14->setMaterial(material);
		collidableBox14->setMesh("cube.obj");
		collidableBox14->setShader(shaderID[0]);
		collidableBox14->addObjectInstance(glm::vec3(18.0f, 4.0f, 4.0f));
		gameObjects.push_back(collidableBox14);
	//Front Right Crates - x4 On Floor...
		//No.15
		GameObject *collidableBox15 = new GameObject(glm::vec3(18.0f, 0.0f, 18.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox15->setTexture("crateTex.bmp");
		collidableBox15->setMaterial(material);
		collidableBox15->setMesh("cube.obj");
		collidableBox15->setShader(shaderID[0]);
		collidableBox15->addObjectInstance(glm::vec3(18.0f, 0.0f, 18.0f));
		gameObjects.push_back(collidableBox15);
		//No.16
		GameObject *collidableBox16 = new GameObject(glm::vec3(18.0f, 0.0f, 14.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox16->setTexture("crateTex.bmp");
		collidableBox16->setMaterial(material);
		collidableBox16->setMesh("cube.obj");
		collidableBox16->setShader(shaderID[0]);
		collidableBox16->addObjectInstance(glm::vec3(18.0f, 0.0f, 14.0f));
		gameObjects.push_back(collidableBox16);
		//No.17
		GameObject *collidableBox17 = new GameObject(glm::vec3(14.0f, 0.0f, 18.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox17->setTexture("crateTex.bmp");
		collidableBox17->setMaterial(material);
		collidableBox17->setMesh("cube.obj");
		collidableBox17->setShader(shaderID[0]);
		collidableBox17->addObjectInstance(glm::vec3(14.0f, 0.0f, 18.0f));
		gameObjects.push_back(collidableBox17);
		//No.18
		GameObject *collidableBox18 = new GameObject(glm::vec3(14.0f, 0.0f, 14.0f), glm::vec3(2, 2, 2), glm::vec3(NULL, NULL, NULL));
		collidableBox18->setTexture("crateTex.bmp");
		collidableBox18->setMaterial(material);
		collidableBox18->setMesh("cube.obj");
		collidableBox18->setShader(shaderID[0]);
		collidableBox18->addObjectInstance(glm::vec3(14.0f, 0.0f, 14.0f));
		gameObjects.push_back(collidableBox18);




	//Ground Planes
	GameObject *groundPlane = new GameObject(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(20, 0.1, 20), glm::vec3(NULL, NULL, NULL));
	groundPlane->setTexture("concrete.bmp");
	groundPlane->setMaterial(material);
	groundPlane->setMesh("cube.obj");
	groundPlane->setShader(shaderID[0]);
	groundPlane->addObjectInstance(glm::vec3(0.0f, -2.0f, 0.0f));
	gameObjects.push_back(groundPlane);


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


	//Gameplay Collectable
	collectable = new GameObject(glm::vec3(-12.0f, 1.5f, -18.0f), glm::vec3(0.5, 1.05, 0.5), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f);
	collectable->setTexture("secretFiles.bmp");
	collectable->setMaterial(material);
	collectable->setMesh("cube.obj");
	collectable->setShader(shaderID[0]);
	collectable->addObjectInstance(glm::vec3(-12.0f, 1.05f, -18.0f));
	gameObjects.push_back(collectable);



	// COLLISIONS
	// Initialise the bounding boxs and make this object something the particle effect can test for colliion with
	collidableBox->initAABB();
	collidableBox2->initAABB();
	collidableBox3->initAABB();
	collidableBox4->initAABB();
	collidableBox5->initAABB();
	collidableBox6->initAABB();
	collidableBox7->initAABB();
	collidableBox8->initAABB();
	collidableBox9->initAABB();
	collidableBox10->initAABB();
	collidableBox11->initAABB();
	collidableBox12->initAABB();
	collidableBox13->initAABB();
	collidableBox14->initAABB();
	collidableBox15->initAABB();
	collidableBox16->initAABB();
	collidableBox17->initAABB();
	collidableBox18->initAABB();
	groundPlane->initAABB();

	Sprinkler->addtCollidableObjects(collidableBox);
	Sprinkler->addtCollidableObjects(collidableBox2);
	Sprinkler->addtCollidableObjects(collidableBox3);
	Sprinkler->addtCollidableObjects(collidableBox4);
	Sprinkler->addtCollidableObjects(collidableBox5);
	Sprinkler->addtCollidableObjects(collidableBox6);
	Sprinkler->addtCollidableObjects(collidableBox7);
	Sprinkler->addtCollidableObjects(collidableBox8);
	Sprinkler->addtCollidableObjects(collidableBox9);
	Sprinkler->addtCollidableObjects(collidableBox10);
	Sprinkler->addtCollidableObjects(collidableBox11);
	Sprinkler->addtCollidableObjects(collidableBox12);
	Sprinkler->addtCollidableObjects(collidableBox13);
	Sprinkler->addtCollidableObjects(collidableBox14);
	Sprinkler->addtCollidableObjects(collidableBox15);
	Sprinkler->addtCollidableObjects(collidableBox16);
	Sprinkler->addtCollidableObjects(collidableBox17);
	Sprinkler->addtCollidableObjects(collidableBox18);
	Sprinkler->addtCollidableObjects(groundPlane);

}