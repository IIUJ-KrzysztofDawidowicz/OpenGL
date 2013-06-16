#include "stdafx.h"
#include "Tekstury.h"

#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
#include <windows.h>               // standardowy plik nag³ówkowy Windows
#include <cstdio>

Tekstury::Tekstury(void)
{
}


Tekstury::~Tekstury(void)
{
}


int Tekstury::LoadGLTexture(char *plik)
{
  unsigned int Texture;
  FILE* img = NULL;

  img = fopen(plik,"rb");
  if (img==NULL)
  {
	MessageBox(NULL, "Nie mo¿na otworzyæ pliku z tekstur¹", "B³¹d", MB_OK);
	return -1;
  }
  unsigned long bWidth = 0;	
  unsigned long bHeight = 0;	
  DWORD size = 0;	

  fseek(img,18,SEEK_SET);
  fread(&bWidth,4,1,img);
  fread(&bHeight,4,1,img);
  fseek(img,0,SEEK_END);
  size = ftell(img) - 54;

  unsigned char *data = (unsigned char*)malloc(size);
  if (data==NULL)
  {
	MessageBox(NULL, "Nie mo¿na zaalokowaæ pamiêci na teksturê", "B³¹d", MB_OK);
	return -1;
  }

  fseek(img,54,SEEK_SET);	// image data
  fread(data,size,1,img);
  fclose(img);

  glGenTextures(1, &Texture);
  glBindTexture(GL_TEXTURE_2D, Texture);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bWidth, bHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  
  free(data);
  return Texture;
}
