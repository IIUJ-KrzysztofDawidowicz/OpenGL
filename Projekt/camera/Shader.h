#pragma once
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
class Shader
{
	GLuint shader_program;
	
	char *ReadAllTextFromFile(char *fname);

		
	bool MakeShader(GLuint shader, char* name);

	GLuint MakeVertexShader(char* name);


	GLuint MakeFragmentShader(char* name);
	void LinkShader(GLuint vertex_shader, GLuint fragment_shader);
public:
	Shader();
	Shader(char* vertexShaderFile, char* fragmentShaderFile);
	~Shader();

	GLuint getId();

	void Delete();
	void BindValue(const GLchar* name, const float value);

	void BindValue(const GLchar* name, const int value);

	void BindValue(const GLchar* name, const GLuint value);

	void BindValueArray(const GLchar* name, const int length, const float values[]);

	void Activate();

	void Deactivate();
};
