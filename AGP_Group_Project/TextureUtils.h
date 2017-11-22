#ifndef TEXTURE
#define TEXTURE

#include "GL/glew.h"
#include "SDL.h"
#include <iostream>
#include <SDL_ttf.h>

namespace TextureUtils
{
	GLuint textToTexture(const char * str, GLuint textID);
	GLuint loadBitmap(char *fname);



	
}

#endif