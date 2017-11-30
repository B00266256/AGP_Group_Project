#include "TextureUtils.h"

namespace TextureUtils
{
	TTF_Font * textFont;

	GLuint textToTexture(const char * str, GLuint textID/*, TTF_Font *font, SDL_Color colour, GLuint &w,GLuint &h */) {
		TTF_Font *font = textFont;
		SDL_Color colour = { 255, 255, 255 };
		SDL_Color bg = { 0, 0, 0 };

		SDL_Surface *stringImage;
		stringImage = TTF_RenderText_Blended(font, str, colour);

		if (stringImage == NULL)
			//exitFatalError("String surface not created.");
			std::cout << "String surface not created." << std::endl;

		GLuint w = stringImage->w;
		GLuint h = stringImage->h;
		GLuint colours = stringImage->format->BytesPerPixel;

		GLuint format, internalFormat;
		if (colours == 4) {   // alpha
			if (stringImage->format->Rmask == 0x000000ff)
				format = GL_RGBA;
			else
				format = GL_BGRA;
		}
		else {             // no alpha
			if (stringImage->format->Rmask == 0x000000ff)
				format = GL_RGB;
			else
				format = GL_BGR;
		}
		internalFormat = (colours == 4) ? GL_RGBA : GL_RGB;

		GLuint texture = textID;

		if (texture == 0) {
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		} //Do this only when you initialise the texture to avoid memory leakage

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, stringImage->w, stringImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, stringImage->pixels);
		glBindTexture(GL_TEXTURE_2D, NULL);

		SDL_FreeSurface(stringImage);
		return texture;
	}


	GLuint loadBitmap(char *fname) {
		GLuint texID;
		glGenTextures(1, &texID); // generate texture ID

								  // load file - using core SDL library
		SDL_Surface *tmpSurface;
		tmpSurface = SDL_LoadBMP(fname);
		if (!tmpSurface) {
			std::cout << "Error loading bitmap" << std::endl;
		}

		// bind texture and set parameters
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		SDL_PixelFormat *format = tmpSurface->format;

		////
		//// Widthand height in number of pixels 
		int width = tmpSurface->w;
		int height = tmpSurface->h;
		////
		////
		GLuint externalFormat, internalFormat;
		if (format->Amask) {
			internalFormat = GL_RGBA;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
		}
		else {
			internalFormat = GL_RGB;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0,
			externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
		return texID;	// return value of texture ID
	}



}