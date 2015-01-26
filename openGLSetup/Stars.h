#ifndef _STARS_H_
#define _STARS_H_

#include <stdlib.h> 
#include <time.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>
#include <vector>
#include <string>
#include <fstream>


class Stars
{
public:
	Stars();
	Stars(int num);
	void Draw();
	~Stars();

private:
	float pos[4];
	float color[4];
	GLuint uiProgramFlat;
	int numStars;

	GLuint VBO;
	//float uv[2];
};


GLuint CreateProgram(const char *a_vertex, const char *a_frag);

GLuint CreateShader(GLenum a_eShaderType, const char *a_strShaderFile);

float* getOrtho(float left, float right, float bottom, float top, float a_fNear, float a_fFar);

//void Create(int amount);

#endif