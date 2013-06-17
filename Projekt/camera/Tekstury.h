#pragma once
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nagłówkowy OpenGL
#include <gl/glu.h>                // plik nagłówkowy dodatkowych aplikacji bibliotek OpenGL
class Tekstury
{
	static int LoadGLTexture(char *plik);
public:
	GLuint tekstura;
	Tekstury(void);
	Tekstury(char* plik)
	{
		tekstura = LoadGLTexture(plik);
	}
	~Tekstury(void);

};

