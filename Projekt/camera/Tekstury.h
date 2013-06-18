#pragma once
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nagłówkowy OpenGL
#include <gl/glu.h>                // plik nagłówkowy dodatkowych aplikacji bibliotek OpenGL
class Tekstura
{
	static int LoadGLTexture(char *plik);
	GLenum tekstureEnum;
	void Bind(GLenum texture)
	{
		glActiveTexture(texture);
		glBindTexture(GL_TEXTURE_2D, tekstura);
		glEnable(GL_TEXTURE_2D);
	}
public:
	GLuint tekstura;
	Tekstura(void);

	Tekstura(char* plik, GLenum tekstureEnum)
	{
		tekstura = LoadGLTexture(plik);
		this->tekstureEnum = tekstureEnum;
	}
	~Tekstura(void);
	void Bind()
	{
		Bind(tekstureEnum);
	}
};

