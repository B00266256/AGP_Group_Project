#include "Scene.h"

Scene::Scene()
{
	

	//model view and camera setup
	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 800.0f / 600.0f, 0.5f, 2000.0f);
	glm::mat4 modelview(1.0);
	mvStack.push(modelview);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	eye = glm::vec3(-15.0f, 5.0f, -20);
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
	loadGroundAndWalls();


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


	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-10, 3, -10));
	shader.setMVP(shaderID[0], mvStack.top());
	shader.setSpotlight(shaderID[0], spotLight, glm::value_ptr(spotLightDirection));
	mvStack.pop();

	//update and draw particles
	glUseProgram(shaderID[1]);
	glBindTexture(GL_TEXTURE_2D, particleTexture);
	//update particle shader
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(),glm::vec3(0,0,0));
	shader.setProjection(shaderID[1], projection);
	shader.setMVP(shaderID[1], mvStack.top());

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(0);
	
	//draw
	Sprinkler->draw();
	Sprinkler->update();

	glDisable(GL_BLEND);
	glDepthMask(1);
	glDepthMask(GL_TRUE);
	

	mvStack.pop();
}


//camer movement
void Scene::update()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W]) eye = moveForward(eye, r, 0.05f);
	if (keys[SDL_SCANCODE_S]) eye = moveForward(eye, r, -0.05f);
	if (keys[SDL_SCANCODE_A]) eye = moveRight(eye, r, -0.05f);
	if (keys[SDL_SCANCODE_D]) eye = moveRight(eye, r, 0.05f);
	if (keys[SDL_SCANCODE_R]) eye.y += 0.1f;
	if (keys[SDL_SCANCODE_F]) eye.y -= 0.1f;

	if (keys[SDL_SCANCODE_COMMA]) r -= 0.3f;
	if (keys[SDL_SCANCODE_PERIOD]) r += 0.3f;

	if (keys[SDL_SCANCODE_1]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	if (keys[SDL_SCANCODE_2]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	at = moveForward(eye, r, 1.0f);
	mvStack.top() = glm::lookAt(eye, at, up);

	glm::vec3 sprinklerPos = moveForward(eye, r, 6.0f);
	Sprinkler->setEmitPos(glm::vec3(sprinklerPos.x, 9.0f, sprinklerPos.z));
}


void Scene::loadGroundAndWalls()
{


	//Test collision detection between AABB and water particles
	GameObject *collidableBox = new GameObject(glm::vec3(-15, 3, -30), glm::vec3(1, 1, 1), glm::vec3(NULL, NULL, NULL));
	collidableBox->setTexture("concrete.bmp");
	collidableBox->setMaterial(material);
	collidableBox->setMesh("cube.obj");
	collidableBox->setShader(shaderID[0]);
	collidableBox->addObjectInstance(glm::vec3(-15, 3, -30));
	gameObjects.push_back(collidableBox);

	// COLLISIONS
	// Initialise the bounding box and make this object something the particle effect can test for colliion with
	collidableBox->initAABB();
	Sprinkler->addtCollidableObjects(collidableBox);


	//Ground Planes
	GameObject *groundPlane = new GameObject(glm::vec3(0, 0, 0), glm::vec3(20, 0.1, 20), glm::vec3(NULL, NULL, NULL));
	groundPlane->setTexture("concrete.bmp");
	groundPlane->setMaterial(material);
	groundPlane->setMesh("cube.obj");
	groundPlane->setShader(shaderID[0]);
	groundPlane->addObjectInstance(glm::vec3(0, 0, 0));
	groundPlane->addObjectInstance(glm::vec3(0, 0, -40));
	groundPlane->addObjectInstance(glm::vec3(0, 0, -80));
	groundPlane->addObjectInstance(glm::vec3(-40, 0, 0));
	groundPlane->addObjectInstance(glm::vec3(-40, 0, -40));
	groundPlane->addObjectInstance(glm::vec3(-40, 0, -80));

	gameObjects.push_back(groundPlane);

	// COLLISIONS
	// Initialise the bounding box and make this object something the particle effect can test for colliion with
	groundPlane->initAABB();
	Sprinkler->addtCollidableObjects(groundPlane);



	//Roof Planes
	GameObject *roofPlane = new GameObject(glm::vec3(0, 0, 0), glm::vec3(20, 0.1, 20), glm::vec3(NULL, NULL, NULL));
	roofPlane->setTexture("concrete.bmp");
	roofPlane->setMaterial(material);
	roofPlane->setMesh("cube.obj");
	roofPlane->setShader(shaderID[0]);
	roofPlane->addObjectInstance(glm::vec3(0, 20, 0));
	roofPlane->addObjectInstance(glm::vec3(0, 20, -40));
	roofPlane->addObjectInstance(glm::vec3(0, 20, -80));
	roofPlane->addObjectInstance(glm::vec3(-40, 20, 0));
	roofPlane->addObjectInstance(glm::vec3(-40, 20, -40));
	roofPlane->addObjectInstance(glm::vec3(-40, 20, -80));

	gameObjects.push_back(roofPlane);


	//Left Wall Planes
	GameObject *walls = new GameObject(glm::vec3(0, 0, 0), glm::vec3(0.3, 10, 20), glm::vec3(NULL, NULL, NULL));
	walls->setTexture("brickwall.bmp");
	walls->setMaterial(material);
	walls->setMesh("cube.obj");
	walls->setShader(shaderID[0]);
	walls->addObjectInstance(glm::vec3(-60, 10, 0));
	walls->addObjectInstance(glm::vec3(-60, 10, -40));
	walls->addObjectInstance(glm::vec3(-60, 10, -80));

	gameObjects.push_back(walls);

	//right Wall Planes
	GameObject *walls2 = new GameObject(glm::vec3(0, 0, 0), glm::vec3(0.3, 10, 20), glm::vec3(0, 1, 0));
	walls2->setTexture("brickwall.bmp");
	walls2->setMaterial(material);
	walls2->setMesh("cube.obj");
	walls2->setShader(shaderID[0]);
	walls2->addObjectInstance(glm::vec3(20, 10, 0));
	walls2->addObjectInstance(glm::vec3(20, 10, -40));
	walls2->addObjectInstance(glm::vec3(20, 10, -80));

	gameObjects.push_back(walls2);

	//Rear Wall Planes
	GameObject *walls3 = new GameObject(glm::vec3(0, 0, 0), glm::vec3(10, 10, 0.3), glm::vec3(NULL, NULL, NULL));
	walls3->setTexture("brickwall.bmp");
	walls3->setMaterial(material);
	walls3->setMesh("cube.obj");
	walls3->setShader(shaderID[0]);
	walls3->addObjectInstance(glm::vec3(-50, 10, -100));
	walls3->addObjectInstance(glm::vec3(-30, 10, -100));
	walls3->addObjectInstance(glm::vec3(-10, 10, -100));
	walls3->addObjectInstance(glm::vec3(10, 10, -100));

	gameObjects.push_back(walls3);

	//Rear Wall Planes
	GameObject *walls4 = new GameObject(glm::vec3(0, 0, 0), glm::vec3(10, 10, 0.3), glm::vec3(NULL, NULL, NULL));
	walls4->setTexture("brickwall.bmp");
	walls4->setMaterial(material);
	walls4->setMesh("cube.obj");
	walls4->setShader(shaderID[0]);
	walls4->addObjectInstance(glm::vec3(-50, 10, 20));
	walls4->addObjectInstance(glm::vec3(-30, 10, 20));
	walls4->addObjectInstance(glm::vec3(-10, 10, 20));
	walls4->addObjectInstance(glm::vec3(10, 10, 20));

	gameObjects.push_back(walls4);

}