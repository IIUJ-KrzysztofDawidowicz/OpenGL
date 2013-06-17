#pragma once
#include "GLee.h"                  // GL Easy Extension library
#include <gl/gl.h>                 // standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>                // plik nag³ówkowy dodatkowych aplikacji bibliotek OpenGL
class Shader
{
	GLuint shader_program;
	
	char *ReadAllTextFromFile(char *fname) {
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

		
	bool MakeShader(GLuint shader, char* name)
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

	GLuint MakeVertexShader(char* name)
	{
		  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		  MakeShader(vertex_shader, name);
  
	  return vertex_shader;
	}


	GLuint MakeFragmentShader(char* name)
	{
	  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		  MakeShader(fragment_shader, name);
  
	  return fragment_shader;
	}	
	void LinkShader(GLuint vertex_shader, GLuint fragment_shader)
	{
		  shader_program = glCreateProgram();
	  glAttachShader(shader_program, vertex_shader);
	  glAttachShader(shader_program, fragment_shader);

	  glLinkProgram(shader_program);
	  glUseProgram(shader_program);
	}
public:

	Shader(char* vertexShaderFile, char* fragmentShaderFile)
	{
		GLuint vertex_shader = MakeVertexShader(vertexShaderFile);
		GLuint fragment_shader = MakeFragmentShader(fragmentShaderFile);
		LinkShader(vertex_shader, fragment_shader);
	}
	~Shader()
	{
		Delete();
	}

	void Delete()
	{
		  glDeleteProgram(shader_program);
	}
	void BindValue(const GLchar* name, const float value)
	{
		GLuint location = glGetUniformLocation(shader_program, name);
		glUniform1f(location, value);
	}

	void BindValue(const GLchar* name, const int value)
	{
		GLuint location = glGetUniformLocation(shader_program, name);
		glUniform1i(location, value);
	}
};
