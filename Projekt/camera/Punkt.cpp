#include "stdafx.h"
#include "Punkt.h"
#include "ManagerTekstur.h"
#ifndef GLEE_INCLUDED
#define GLEE_INCLUDED
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL

#endif // !GLEE_INCLUDED

//
//Punkt::NUM_POINTS= 300;
//Punkt::PARTICLE_LIFE = 1000; //Czas ¿ycia cz¹stki
//Punkt::MAX_VELOCITY = 100;

Punkt::Punkt()
{
}

Punkt::Punkt(const float newstartTime, ManagerTekstur tekstury, const short NUM_TEKSTUR, const short OFFSET_TEKSTUR)
{
		//Czas zaistnienia
	startTime = newstartTime;

	//Prêdkoœæ
	velocity[0] = ((float)(rand()%MAX_VELOCITY))/10; //Prêdkoœæ x
	velocity[1] = 5 + ((float)(rand()%MAX_VELOCITY))/20; //Prêdkoœæ y
	velocity[2] = -((float)(rand()%MAX_VELOCITY))/10; //Prêdkoœæ z
	if(rand()%2 == 0) //Przpadkowo odwracamy prêdkoœæ x.
		velocity[0] = -velocity[0];
	if(rand()%2 == 0) //Przpadkowo odwracamy prêdkoœæ z.
		velocity[2] = -velocity[2];
	//if(rand()%2 == 0) //Punkty o dodatniej prêdkoœci z szybko znikaj¹ z ekranu
	//	velocity[2] = -velocity[2];

	//Kolor
	const float minColor = 0.125; //Minimalna wartoœc koloru, czyni kolory bardziej pastelowymi.
	color[0]= minColor + ((float)(rand()%8))/4;
	color[1]= minColor + ((float)(rand()%8))/4;
	color[2]= minColor + ((float)(rand()%8))/4;

	//Kszta³t
	int indexTekstury = OFFSET_TEKSTUR + rand()%NUM_TEKSTUR;
	tekstura = tekstury[indexTekstury];
}

GLvoid Punkt::Rysuj(Shader shader, const float& currentTime)
{
	//Przekazujemy zmienne
	//GLuint location = glGetUniformLocation(shader_program, "time");
	//glUniform1f(location, currentTime-startTime);
	//location = glGetUniformLocation(shader_program,"velocity");
	//glUniform3f(location, velocity[0],velocity[1],velocity[2]);
	//location = glGetUniformLocation(shader_program,"color");
	//glUniform4f(location, color[0],color[1],color[2],1-(currentTime-startTime)/512);
	//glUniform3f(location, color[0],color[1],color[2]);
	shader.BindValue("time", currentTime - startTime);
	shader.BindValueArray("velocity", 3, velocity);
	shader.BindValueArray("color", 3, color);

	//glBindTexture(GL_TEXTURE_2D, tekstura); //Ustalamy, która tekstura jest zbindowana
	tekstura.Bind();

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE_EXT);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glBegin(GL_POINTS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f,  0.0f, 0.0f); 
	glEnd();
	glDisable(GL_POINT_SPRITE);
}

float Punkt::getStart()
{
	return startTime;
}

Punkt::~Punkt(void)
{
}
