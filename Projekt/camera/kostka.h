#pragma once
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL

//extern GLfloat SpecularLight[];
//extern GLfloat DiffuseLight[];
//extern GLfloat AmbientLight[];
//extern GLfloat LightPosition[];
//extern GLfloat DiffuseMaterial[]; 
//extern GLfloat SpecularMaterial[]; 
//extern GLfloat mShininess;


GLvoid glDrawCube(const float size);

//Struktura dla danych per-vertex
struct Vertex
{
	float vertex[3];        //Vertex
	float normal[3];     //Normal
	float texcoord[2];         //Texcoord0
};

class Kostka
{
	Vertex* data;

	Vertex* InitData(const float size);
public:
	Kostka();
	Kostka(const float size);
	~Kostka();

	void Rysuj();
};