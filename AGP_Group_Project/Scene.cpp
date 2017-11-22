#include "Scene.h"

Scene::Scene()
{
	//model view and camera setup
	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 800.0f / 600.0f, 0.5f, 2000.0f);
	glm::mat4 modelview(1.0);
	mvStack.push(modelview);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	eye = glm::vec3(-15.0f, 5.0f, 10.0f);
	at = glm::vec3(0.0f, 1.0f, -1.0f);
	mvStack.top() = glm::lookAt(eye, at, up);

	renderer = new Renderer();

	//Phong tex shader program
	shaderID[0] = shader.init("phong-tex.vert", "phong-tex.frag");
	shader.setLight(shaderID[0], light);
	shader.setMaterial(shaderID[0], material);
	shader.setProjection(shaderID[0], projection);
	shader.setMVP(shaderID[0], mvStack.top());

	//Particle Shader
	shaderID[1] = shader.init("Particle.vert", "Particle.frag");
	shader.setProjection(shaderID[1], projection);
	shader.setMVP(shaderID[1], mvStack.top());

	//init initialises all game objects and pushes them all into a vector for passing to the renderer.
	init();
}



void Scene::init()
{
	loadGroundAndWalls();

	//particle effect
	particleTest = new ParticleEffect(100);


}

glm::vec3 Scene::moveForward(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::sin(r*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(r*DEG_TO_RADIAN));
}

glm::vec3 Scene::moveRight(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::cos(r*DEG_TO_RADIAN), pos.y, pos.z + d*std::sin(r*DEG_TO_RADIAN));
}

//camer movement
void Scene::update()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W]) eye = moveForward(eye, r, 0.1f);
	if (keys[SDL_SCANCODE_S]) eye = moveForward(eye, r, -0.1f);
	if (keys[SDL_SCANCODE_A]) eye = moveRight(eye, r, -0.1f);
	if (keys[SDL_SCANCODE_D]) eye = moveRight(eye, r, 0.1f);
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
}

//send out pile of game object to the renderer along with the model view stack
void Scene::draw()
{
	// clear the screen
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//update and draw particles
	glUseProgram(shaderID[1]);
	particleTest->update();
	particleTest->draw();

	renderer->draw(gameObjects, mvStack);

	
}



void Scene::loadGroundAndWalls()
{
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