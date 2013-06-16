#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL

#if !defined KOSTKA
#define KOSTKA

GLfloat SpecularLight[] = {1,1,1};
GLfloat DiffuseLight[] = {1,1,1};
GLfloat AmbientLight[] = {1,1,1};
GLfloat LightPosition[] = {0.5,0.5,1};
GLfloat DiffuseMaterial[] = {1.0, 0.0, 0.0}; 
GLfloat SpecularMaterial[] = {1.0, 1.0, 1.0}; 
GLfloat mShininess = 120;

//Struktura dla danych per-vertex
typedef struct Vertex
{
	float vertex[3];        //Vertex
	float normal[3];     //Normal
	float texcoord[2];         //Texcoord0
};



//GLfloat vertices[] = {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
//					  1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
//					  1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
//					  -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
//					  -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
//					  1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5
//
//const GLshort squareTextureCoords[] = {
//		// Front face
//		0, 1,       // top left
//		0, 0,       // bottom left
//		1, 0,       // bottom right
//		1, 1,       // top right
//
//		// Top face
//		0, 1,       // top left
//		0, 0,       // bottom left
//		1, 0,       // bottom right
//		1, 1,       // top right
//
//		// Rear face
//		0, 1,       // top left
//		0, 0,       // bottom left
//		1, 0,       // bottom right
//		1, 1,       // top right
//
//		// Bottom face
//		0, 1,       // top left
//		0, 0,       // bottom left
//		1, 0,       // bottom right
//		1, 1,       // top right
//
//		// Left face
//		0, 1,       // top left
//		0, 0,       // bottom left
//		1, 0,       // bottom right
//		1, 1,       // top right
//
//		// Right face
//		0, 1,       // top left
//		0, 0,       // bottom left
//		1, 0,       // bottom right
//		1, 1,       // top right
//	};


GLvoid glDrawCube()                 // Draw A Cube
{
	
	glColor3f(0.8f, 0.6f, 0.6f);     // lekko fioletowa
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &mShininess);



Vertex data[] = 
{
	
	

	//    glBegin(GL_QUADS);          // Start Drawing Quads
	//    // Front Face
	//    glNormal3f( 0.0f, 0.0f, 1.0f);      // Normal Facing Forward
	{
		{-1.0f, -1.0f,  1.0f}, 
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f}
	},
	//    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
	{
		{1.0f, -1.0f,  1.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f}
	},
	//    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
	
	{
		{1.0f,  1.0f,  1.0f},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f}
	},
	//    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
	{
		{-1.0f,  1.0f,  1.0f},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 0.0f}
	},
	//    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
	//    // Back Face
	//    glNormal3f( 0.0f, 0.0f,-1.0f);      // Normal Facing Away
	{
		{-1.0f,  -1.0f,  -1.0f},
		{0.0f, 0.0f,-1.0f},
		{1.0f, 0.0f}
	},
	//    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
	{
		{-1.0f,  1.0f, -1.0f},	//vertex
		{0.0f, 0.0f,-1.0f},		//normal
		{1.0f, 1.0f}			//texture
	},
	//    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
	{
		{1.0f,  1.0f, -1.0f},	//vertex
		{0.0f, 0.0f,-1.0f},		//normal
		{0.0f, 1.0f}			//texture
	},
	//    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
	{
		{1.0f,  -1.0f, -1.0f},	//vertex
		{0.0f, 0.0f,-1.0f},		//normal
		{0.0f, 0.0f}			//texture
	},
	//    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
	//    // Top Face
	//    glNormal3f( 0.0f, 1.0f, 0.0f);      // Normal Facing Up
	{
		{-1.0f,  1.0f, -1.0f},	//vertex
		{ 0.0f, 1.0f, 0.0f},		//normal
		{0.0f, 1.0f}			//texture
	},
	//    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
	{
		{-1,  1,  1},	//vertex
		{ 0, 1, 0},		//normal
		{0, 0}			//texture
	},
	//    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
	{
		{1,  1,  1},	//vertex
		{ 0, 1, 0},		//normal
		{1, 0}			//texture
	},
	//    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
	{
		{1,  1,  -1},	//vertex
		{ 0, 1, 0},		//normal
		{1, 1}			//texture
	},
	//    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
	//    // Bottom Face
	//    glNormal3f( 0.0f,-1.0f, 0.0f);      // Normal Facing Down
	{
		{-1, -1, -1},	//vertex
		{ 0, -1, 0},		//normal
		{1, 1}			//texture
	},
	//    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
	{
		{1, -1, -1},	//vertex
		{ 0, -1, 0},		//normal
		{0, 1}			//texture
	},
	//    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
	{
		{1, -1, 1},	//vertex
		{ 0, -1, 0},		//normal
		{0, 0}			//texture
	},
	//    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
	{
		{-1, -1, 1},	//vertex
		{ 0, -1, 0},		//normal
		{1, 0}			//texture
	},
	//    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
	//    // Right face
	//    glNormal3f( 1.0f, 0.0f, 0.0f);      // Normal Facing Right
	{
		{1, -1, -1},	//vertex
		{ 1, 0, 0},		//normal
		{1, 0}			//texture
	},
	//    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
	{
		{1, 1, -1},	//vertex
		{ 1, 0, 0},		//normal
		{1, 1}			//texture
	},
	//    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
	{
		{1, 1, 1},	//vertex
		{ 1, 0, 0},		//normal
		{0, 1}			//texture
	},
	//    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
	{
		{1, -1, 1},	//vertex
		{ 1, 0, 0},		//normal
		{0, 0}			//texture
	},
	//    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
	//    // Left Face
	//    glNormal3f(-1.0f, 0.0f, 0.0f);      // Normal Facing Left
	{
		{-1, -1, -1},	//vertex
		{ -1, 0, 0},		//normal
		{0, 0}			//texture
	},
	//    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
	{
		{-1, -1, 1},	//vertex
		{ -1, 0, 0},		//normal
		{1, 0}			//texture
	},
	//    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
	{
		{-1, 1, 1},	//vertex
		{ -1, 0, 0},		//normal
		{1, 1}			//texture
	},
	//    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
	{
		{-1, 1, -1},	//vertex
		{ -1, 0, 0},		//normal
		{0, 1}			//texture
	},
	//    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
	//glEnd();                    // Done Drawing Quads
	};// activate and specify pointer to vertex array
glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &data[0].vertex);
	
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &data[0].normal);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &data[0].texcoord);
// draw a cube
glDrawArrays(GL_QUADS, 0, 24);

// deactivate vertex arrays after drawing
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
}

#endif