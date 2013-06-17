#pragma once
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
class Tekstura
{
	static int LoadGLTexture(char *plik);
public:
	GLuint tekstura;
	Tekstura(void);
	Tekstura(char* plik)
	{
		tekstura = LoadGLTexture(plik);
	}
	~Tekstura(void);
	void Bind(GLenum texture = GL_TEXTURE0)
{
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, tekstura);
  glEnable(GL_TEXTURE_2D);
}
};

