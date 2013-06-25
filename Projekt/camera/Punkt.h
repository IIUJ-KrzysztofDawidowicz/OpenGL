#pragma once
#ifndef GLEE_INCLUDED
#define GLEE_INCLUDED
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag��wkowy OpenGL

#endif // !GLEE_INCLUDED
#include "Tekstury.h"
#include "ManagerTekstur.h"
#include "Shader.h"

class Punkt
{
	float startTime; //Czas kiedy cz�stka zaczyna istnie�
	GLfloat velocity[3]; //Pr�dko�� pocz�tkowa
	GLfloat color[3]; //Kolor
	Tekstura tekstura; //Numer kszta�tu
public:
	
	static const int PARTICLE_LIFE = 5; //Czas �ycia cz�stki
	static const int MAX_VELOCITY = 100;

	Punkt();
	Punkt(const float newStart, ManagerTekstur tekstura, const short NUM_TEKSTUR, const short OFFSET_TEKSTUR = 0);
	GLvoid Rysuj(Shader shader, const float& currentTime);
	float getStart();
	~Punkt(void);
};

