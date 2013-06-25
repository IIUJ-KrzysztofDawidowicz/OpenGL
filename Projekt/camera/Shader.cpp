#include "Shader.h"
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
#include <iostream>

char* Shader::ReadAllTextFromFile(char *fname) {
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fname == NULL) return NULL;

	fp = fopen(fname,"rt");

	if (fp == NULL) return NULL;
	
	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	if (count > 0) {
		content = new char[sizeof(char) * (count+1)];
		count = fread(content,sizeof(char), count, fp);
		content[count] = '\0';
	}
	fclose(fp);

	return content;
}

		
bool Shader::MakeShader(GLuint shader, char* name)
{
	if (shader == 0)
	{
	MessageBox(NULL, "Nie ma szejderow", NULL, MB_OK);
	return false;
	}
	char *text = ReadAllTextFromFile(name);
	if (text == NULL) {
	MessageBox(NULL, "Nie mo¿e za³adowaæ vertex-szejdera", NULL, MB_OK);
	return 0;
	}
	const char * temp = text;
	glShaderSource(shader, 1, &temp, NULL);
	delete [] text;
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
	MessageBox(NULL, "B³¹d kompilacji vertex-szejdera", NULL, MB_OK);
	return 0;
	}
}

GLuint Shader::MakeVertexShader(char* name)
{
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		MakeShader(vertex_shader, name);
  
	return vertex_shader;
}


GLuint Shader::MakeFragmentShader(char* name)
{
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		MakeShader(fragment_shader, name);
  
	return fragment_shader;
}	
void Shader::LinkShader(GLuint vertex_shader, GLuint fragment_shader)
{
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);
	//Activate();
}
Shader::Shader() {}
Shader::Shader(char* vertexShaderFile, char* fragmentShaderFile)
{
	GLuint vertex_shader = MakeVertexShader(vertexShaderFile);
	GLuint fragment_shader = MakeFragmentShader(fragmentShaderFile);
	LinkShader(vertex_shader, fragment_shader);
}
Shader::~Shader()
{
	//Delete(); Powoduje delecjê przy znikniêciu tymczasowych obiektów.
}

GLuint Shader::getId()
{
	return shader_program;
}

void Shader::Delete()
{
		glDeleteProgram(shader_program);
}
void Shader::BindValue(const GLchar* name, const float value)
{
	GLuint location = glGetUniformLocation(shader_program, name);
	glUniform1f(location, value);
}

void Shader::BindValue(const GLchar* name, const int value)
{
	GLuint location = glGetUniformLocation(shader_program, name);
	glUniform1i(location, value);
}

void Shader::BindValue(const GLchar* name, const GLuint value)
{
	GLuint location = glGetUniformLocation(shader_program, name);
	glUniform1uiEXT(location, value);
}

void Shader::BindValueArray(const GLchar* name, const int length, const float values[])
{
	GLuint location = glGetUniformLocation(shader_program, name);
	switch(length)
	{
		case 1:
			glUniform1i(location, values[0]);
			break;
		case 2:
			glUniform2f(location, values[0], values[1]);
			break;
		case 3:
			glUniform3f(location, values[0], values[1], values[2]);
			break;
		case 4:
			glUniform4f(location, values[0], values[1], values[2], values[3]);
			break;
	}
}

void Shader::Activate()
{
	glUseProgram(shader_program);
}

void Shader::Deactivate()
{
	glUseProgram(0);
}