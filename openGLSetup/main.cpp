#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW\glfw3.h>

#include <vector>
#include <string>
#include <fstream>

GLuint CreateShader(GLenum a_eShaderType, const char *a_strShaderFile)
{
	std::string strShaderCode;
	//open shader file
	std::ifstream shaderStream(a_strShaderFile);
	//if that worked ok, load file line by line
	if (shaderStream.is_open())
	{
		std::string Line = "";
		while (std::getline(shaderStream, Line))
		{
			strShaderCode += "\n" + Line;
		}
		shaderStream.close();
	}

	//convert to cstring
	char const *szShaderSourcePointer = strShaderCode.c_str();

	//create shader ID
	GLuint uiShader = glCreateShader(a_eShaderType);
	//load source code
	glShaderSource(uiShader, 1, &szShaderSourcePointer, NULL);

	//compile shader
	glCompileShader(uiShader);

	//check for compilation errors and output them
	GLint iStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iStatus);
	if (iStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(uiShader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (a_eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return uiShader;
}

GLuint CreateProgram(const char *a_vertex, const char *a_frag)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, a_vertex));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, a_frag));

	//create shader program ID
	GLuint uiProgram = glCreateProgram();

	//attach shaders
	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
		glAttachShader(uiProgram, *shader);

	//link program
	glLinkProgram(uiProgram);

	//check for link errors and output them
	GLint status;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(uiProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(uiProgram, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glDetachShader(uiProgram, *shader);
		glDeleteShader(*shader);
	}

	return uiProgram;
}

float* getOrtho(float left, float right, float bottom, float top, float a_fNear, float a_fFar)
{
	//to correspond with mat4 in the shader
	//ideally this function would be part of your matrix class
	//however I wasn't willing to write your matrix class for you just to show you this
	//so here we are in array format!
	//add this to your matrix class as a challenge if you like!
	float* toReturn = new float[12];
	toReturn[0] = 2.0 / (right - left);;
	toReturn[1] = toReturn[2] = toReturn[3] = toReturn[4] = 0;
	toReturn[5] = 2.0 / (top - bottom);
	toReturn[6] = toReturn[7] = toReturn[8] = toReturn[9] = 0;
	toReturn[10] = 2.0 / (a_fFar - a_fNear);
	toReturn[11] = 0;
	toReturn[12] = -1 * ((right + left) / (right - left));
	toReturn[13] = -1 * ((top + bottom) / (top - bottom));
	toReturn[14] = -1 * ((a_fFar + a_fNear) / (a_fFar - a_fNear));
	toReturn[15] = 1;
	return toReturn;
}

const float vertexPositions[] =
{
	1024 / 2.0, 720 / 2.0 + 10.0f, 0.0f, 1.0f,
	1024 / 2.0 - 10.0f, 720 / 2.0 - 10.0f, 0.0f, 1.0f,
	1024 / 2.0 + 10.0f, 720 / 2.0 - 10.0f, 0.0f, 1.0f,
	1024 / 2.0, 720 / 2.0 - 30.0f, 0.0f, 1.0f,
};

const float vertexColours[] =
{
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
};

const float vertexData[] =
{
	1024 / 2.0, 720 / 2.0 + 10.0f, 0.0f, 1.0f,
	1024 / 2.0 - 10.0f, 720 / 2.0 - 10.0f, 0.0f, 1.0f,
	1024 / 2.0 + 10.0f, 720 / 2.0 - 10.0f, 0.0f, 1.0f,
	1024 / 2.0, 720 / 2.0 - 30.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
};

struct Vertex
{
	float fPositions[4];
	float fColours[4];
};

int main()
{
	//Initialize GLFW
	if (!glfwInit())
		return -1;

	//create window 
	GLFWwindow* window;
	window = glfwCreateWindow(1024,720, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make  window's context current
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		//ohh no it didnt start up!
		glfwTerminate();
		return -1;
	}

	Vertex* myShape = new Vertex[3];
	myShape[0].fPositions[0] = 0.0f;
	myShape[0].fPositions[1] = 0.03f;
	myShape[1].fPositions[0] = -0.025f;
	myShape[1].fPositions[1] = -0.05f;
	myShape[2].fPositions[0] = 0.025f;
	myShape[2].fPositions[1] = -0.05f;
	for (int i = 0; i < 3; i++)
	{
		myShape[i].fPositions[2] = 0.0f;
		myShape[i].fPositions[3] = 1.0f;
		myShape[i].fColours[0] = 0.0f;
		myShape[i].fColours[1] = 0.0f;
		myShape[i].fColours[2] = 1.0f;
		myShape[i].fColours[3] = 1.0f;
	}


	GLuint VBO;

	glGenBuffers(1, &VBO);

	if (VBO != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*3, NULL, GL_STATIC_DRAW);

	}

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	//create shader program
	GLuint uiProgramFlat = CreateProgram("VertexShader.glsl", "FlatFragmentShader.glsl");

	//find the position of the matrix variable in the shader so we can send info there later
	GLuint MatrixIDFlat = glGetUniformLocation(uiProgramFlat, "MVP");

	//set up the mapping of the screen to pixel co-ordinates. Try changing these values to see what happens.
	float *orthographicProjection = getOrtho(0, 1024, 0, 720, 0, 100);



	//loop until window cloases
	while (!glfwWindowShouldClose(window))
	{
		//DRAW SHIT!
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//enable shaders
		glUseProgram(uiProgramFlat);

		//glBindBuffer(GL_ARRAY_BUFFER, VBO);

		//send our orthographic projection info to the shader
		glUniformMatrix4fv(MatrixIDFlat, 1, GL_FALSE, orthographicProjection);

		//enable the vertex array state, since we're sending in an array of vertices
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);

		//specify where our vertex array is, how many components each vertex has, 
		//the data type of each component and whether the data is normalised or not
		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vertexPositions);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vertexColours);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float)*16));

		//draw to the screen
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//spaw front and back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	
	return 0;
}