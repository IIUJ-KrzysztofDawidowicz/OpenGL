#pragma once
#ifndef GLEE_INCLUDED
#define GLEE_INCLUDED
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL

#endif // !GLEE_INCLUDED
#include "Tekstury.h"
#include "ManagerTekstur.h"
#include "Shader.h"

class Punkt
{
	float startTime; //Czas kiedy cz¹stka zaczyna istnieæ
	GLfloat velocity[3]; //Prêdkoœæ pocz¹tkowa
	GLfloat color[3]; //Kolor
	Tekstura tekstura; //Numer kszta³tu
public:
	
	static const int PARTICLE_LIFE = 5; //Czas ¿ycia cz¹stki
	static const int MAX_VELOCITY = 100;

	Punkt();
	Punkt(const float newStart, ManagerTekstur tekstura, const short NUM_TEKSTUR, const short OFFSET_TEKSTUR = 0);
	GLvoid Rysuj(Shader shader, const float& currentTime);
	float getStart();
	~Punkt(void);
};

