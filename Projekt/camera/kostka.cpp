#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
#include "kostka.h"


Kostka::Kostka(): data(InitData(1.0)) {}
Kostka::Kostka(const float size): data(InitData(size)) {}
Kostka::~Kostka() {}

Vertex* Kostka::InitData(const float size)
{
	Vertex data[] = 
	{
	
	

		// Front Face
		{
			{-size, -size,  size}, 
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f}
		},
		// Bottom Left Of The Texture and Quad
		{
			{size, -size,  size},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f}
		},
		// Bottom Right Of The Texture and Quad
	
		{
			{size,  size,  size},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f}
		},
		// Top Right Of The Texture and Quad
		{
			{-size,  size,  size},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 0.0f}
		},
		// Top Left Of The Texture and Quad
		// Back Face
		// Normal Facing Away
		{
			{-size,  -size,  -size},
			{0.0f, 0.0f,-1.0f},
			{1.0f, 0.0f}
		},
		// Bottom Right Of The Texture and Quad
		{
			{-size,  size, -size},	//vertex
			{0.0f, 0.0f,-1.0f},		//normal
			{1.0f, 1.0f}			//texture
		},
		// Top Right Of The Texture and Quad
		{
			{size,  size, -size},	//vertex
			{0.0f, 0.0f,-1.0f},		//normal
			{0.0f, 1.0f}			//texture
		},
		// Top Left Of The Texture and Quad
		{
			{size,  -size, -size},	//vertex
			{0.0f, 0.0f,-1.0f},		//normal
			{0.0f, 0.0f}			//texture
		},
		// Bottom Left Of The Texture and Quad
		// Top Face
		// Normal Facing Up
		{
			{-size,  size, -size},	//vertex
			{ 0.0f, 1.0f, 0.0f},	//normal
			{0.0f, 1.0f}			//texture
		},
		// Top Left Of The Texture and Quad
		{
			{-size,  size,  size},	//vertex
			{ 0, 1, 0},		//normal
			{0, 0}			//texture
		},
		// Bottom Left Of The Texture and Quad
		{
			{size,  size,  size},	//vertex
			{ 0, 1, 0},		//normal
			{1, 0}			//texture
		},
		// Bottom Right Of The Texture and Quad
		{
			{size,  size,  -size},	//vertex
			{ 0, 1, 0},		//normal
			{1, 1}			//texture
		},
		// Top Right Of The Texture and Quad
		// Bottom Face
		// Normal Facing Down
		{
			{-size, -size, -size},	//vertex
			{ 0, -1, 0},	//normal
			{1, 1}			//texture
		},
		// Top Right Of The Texture and Quad
		{
			{size, -size, -size},	//vertex
			{ 0, -1, 0},	//normal
			{0, 1}			//texture
		},
		// Top Left Of The Texture and Quad
		{
			{size, -size, size},		//vertex
			{ 0, -1, 0},	//normal
			{0, 0}			//texture
		},
		// Bottom Left Of The Texture and Quad
		{
			{-size, -size, size},	//vertex
			{ 0, -1, 0},	//normal
			{1, 0}			//texture
		},
		// Bottom Right Of The Texture and Quad
		// Right face
		// Normal Facing Right
		{
			{size, -size, -size},	//vertex
			{ 1, 0, 0},		//normal
			{1, 0}			//texture
		},
		// Bottom Right Of The Texture and Quad
		{
			{size, size, -size},	//vertex
			{ 1, 0, 0},		//normal
			{1, 1}			//texture
		},
		// Top Right Of The Texture and Quad
		{
			{size, size, size},	//vertex
			{ 1, 0, 0},		//normal
			{0, 1}			//texture
		},
		// Top Left Of The Texture and Quad
		{
			{size, -size, size},	//vertex
			{ 1, 0, 0},		//normal
			{0, 0}			//texture
		},
		// Bottom Left Of The Texture and Quad
		// Left Face
		// Normal Facing Left
		{
			{-size, -size, -size},	//vertex
			{ -1, 0, 0},		//normal
			{0, 0}			//texture
		},
		// Bottom Left Of The Texture and Quad
		{
			{-size, -size, size},	//vertex
			{ -1, 0, 0},		//normal
			{1, 0}			//texture
		},
		// Bottom Right Of The Texture and Quad
		{
			{-size, size, size},	//vertex
			{ -1, 0, 0},		//normal
			{1, 1}			//texture
		},
		// Top Right Of The Texture and Quad
		{
			{-size, size, -size},	//vertex
			{ -1, 0, 0},		//normal
			{0, 1}			//texture
		},
		// Top Left Of The Texture and Quad
		// Done Drawing Quads
	};

	return data;
}

void Kostka::Rysuj()
{
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

#if !defined KOSTKA
#define KOSTKA

//GLfloat SpecularLight[] = {1,1,1};
//GLfloat DiffuseLight[] = {1,1,1};
//GLfloat AmbientLight[] = {1,1,1};
//GLfloat LightPosition[] = {0.5,0.5,1};
//GLfloat DiffuseMaterial[] = {1.0, 0.0, 0.0}; 
//GLfloat SpecularMaterial[] = {1.0, 1.0, 1.0}; 
//GLfloat mShininess = 128;


GLvoid glDrawCube(const float size = 2.0f)                 // Draw A Cube
{
	
	//glColor3f(0.8f, 0.6f, 0.6f);     // lekko fioletowa
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SpecularMaterial);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &mShininess);

	//static float size = 2.0f;

static Vertex data[] = 
{
	
	

	// Front Face
	{
		{-size, -size,  size}, 
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f}
	},
	// Bottom Left Of The Texture and Quad
	{
		{size, -size,  size},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f}
	},
	// Bottom Right Of The Texture and Quad
	
	{
		{size,  size,  size},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f}
	},
	// Top Right Of The Texture and Quad
	{
		{-size,  size,  size},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 0.0f}
	},
	// Top Left Of The Texture and Quad
	// Back Face
	// Normal Facing Away
	{
		{-size,  -size,  -size},
		{0.0f, 0.0f,-1.0f},
		{1.0f, 0.0f}
	},
	// Bottom Right Of The Texture and Quad
	{
		{-size,  size, -size},	//vertex
		{0.0f, 0.0f,-1.0f},		//normal
		{1.0f, 1.0f}			//texture
	},
	// Top Right Of The Texture and Quad
	{
		{size,  size, -size},	//vertex
		{0.0f, 0.0f,-1.0f},		//normal
		{0.0f, 1.0f}			//texture
	},
	// Top Left Of The Texture and Quad
	{
		{size,  -size, -size},	//vertex
		{0.0f, 0.0f,-1.0f},		//normal
		{0.0f, 0.0f}			//texture
	},
	// Bottom Left Of The Texture and Quad
	// Top Face
	// Normal Facing Up
	{
		{-size,  size, -size},	//vertex
		{ 0.0f, 1.0f, 0.0f},	//normal
		{0.0f, 1.0f}			//texture
	},
	// Top Left Of The Texture and Quad
	{
		{-size,  size,  size},	//vertex
		{ 0, 1, 0},		//normal
		{0, 0}			//texture
	},
	// Bottom Left Of The Texture and Quad
	{
		{size,  size,  size},	//vertex
		{ 0, 1, 0},		//normal
		{1, 0}			//texture
	},
	// Bottom Right Of The Texture and Quad
	{
		{size,  size,  -size},	//vertex
		{ 0, 1, 0},		//normal
		{1, 1}			//texture
	},
	// Top Right Of The Texture and Quad
	// Bottom Face
	// Normal Facing Down
	{
		{-size, -size, -size},	//vertex
		{ 0, -1, 0},	//normal
		{1, 1}			//texture
	},
	// Top Right Of The Texture and Quad
	{
		{size, -size, -size},	//vertex
		{ 0, -1, 0},	//normal
		{0, 1}			//texture
	},
	// Top Left Of The Texture and Quad
	{
		{size, -size, size},		//vertex
		{ 0, -1, 0},	//normal
		{0, 0}			//texture
	},
	// Bottom Left Of The Texture and Quad
	{
		{-size, -size, size},	//vertex
		{ 0, -1, 0},	//normal
		{1, 0}			//texture
	},
	// Bottom Right Of The Texture and Quad
	// Right face
	// Normal Facing Right
	{
		{size, -size, -size},	//vertex
		{ 1, 0, 0},		//normal
		{1, 0}			//texture
	},
	// Bottom Right Of The Texture and Quad
	{
		{size, size, -size},	//vertex
		{ 1, 0, 0},		//normal
		{1, 1}			//texture
	},
	// Top Right Of The Texture and Quad
	{
		{size, size, size},	//vertex
		{ 1, 0, 0},		//normal
		{0, 1}			//texture
	},
	// Top Left Of The Texture and Quad
	{
		{size, -size, size},	//vertex
		{ 1, 0, 0},		//normal
		{0, 0}			//texture
	},
	// Bottom Left Of The Texture and Quad
	// Left Face
	// Normal Facing Left
	{
		{-size, -size, -size},	//vertex
		{ -1, 0, 0},		//normal
		{0, 0}			//texture
	},
	// Bottom Left Of The Texture and Quad
	{
		{-size, -size, size},	//vertex
		{ -1, 0, 0},		//normal
		{1, 0}			//texture
	},
	// Bottom Right Of The Texture and Quad
	{
		{-size, size, size},	//vertex
		{ -1, 0, 0},		//normal
		{1, 1}			//texture
	},
	// Top Right Of The Texture and Quad
	{
		{-size, size, -size},	//vertex
		{ -1, 0, 0},		//normal
		{0, 1}			//texture
	},
	// Top Left Of The Texture and Quad
	// Done Drawing Quads
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