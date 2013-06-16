#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nagłówkowy OpenGL
#include <gl/glu.h>                // plik nagłówkowy dodatkowych aplikacji bibliotek OpenGL

extern GLfloat SpecularLight[];
extern GLfloat DiffuseLight[];
extern GLfloat AmbientLight[];
extern GLfloat LightPosition[];
extern GLfloat DiffuseMaterial[]; 
extern GLfloat SpecularMaterial[]; 
extern GLfloat mShininess;


GLvoid glDrawCube();