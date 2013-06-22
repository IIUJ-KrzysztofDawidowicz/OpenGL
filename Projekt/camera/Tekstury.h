#pragma once
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
class Tekstura
{
	static int LoadGLTexture(char *plik);
	GLenum tekstureEnum;
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
		glActiveTexture(tekstureEnum);
		glBindTexture(GL_TEXTURE_2D, tekstura);
		glEnable(GL_TEXTURE_2D);
	}

	void Unbind()
	{
		glActiveTexture(tekstureEnum);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
};

