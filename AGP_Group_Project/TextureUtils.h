//TextureUtils.h
//AGP Group Project B00266256, B00286864.
//"Include for methods pertaining to texture loading (Bitmap, PNG, SDLttf)."

#ifndef TEXTURE
#define TEXTURE

#include "GL/glew.h"
#include "SDL.h"
#include <iostream>
#include <SDL_ttf.h>
#include <SDL_image.h>

namespace TextureUtils
{
	GLuint textToTexture(const char * str, GLuint textID);
	GLuint loadBitmap(char *fname);
	GLuint loadPNG(char *fname);	
}
#endif