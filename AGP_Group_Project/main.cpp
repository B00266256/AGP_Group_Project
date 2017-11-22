#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#include "Scene.h"
#include "SDL.h"
#include "GL/glew.h"

#include<iostream>
using namespace std;


SDL_Window * window;
SDL_GLContext glContext;

//Defines the context and initialises GLEW
void setupRenderContext()
{

	//Request an OpenGL 3.0 context.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);										
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);											 
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);									

																						
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
		std::cout << "GLEW not initialized correctly" << std::endl;

	cout << glGetString(GL_VERSION) << endl;										

}


//initialises SDL and creates a window with openGL context
void createWindow()
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0)														
	{
		std::cout << "SDL failed" << std::endl;
	}
	else
	{

		 window = SDL_CreateWindow("iron rifts", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,	
			800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		glContext = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, glContext);

		if (!window)			
			std::cout << "Window not initialized correctly" << std::endl;										

																													
	}

}

int main(int argc, char *argv[])
{
	createWindow();
	setupRenderContext();

	Scene *ourScene = new Scene();
	ourScene->init();

	bool running = true;
	SDL_Event sdlEvent; 

	while (running)
	{
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT)
				running = false;
		}

		ourScene->update();
		ourScene->draw();

		SDL_GL_SwapWindow(window); 
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}