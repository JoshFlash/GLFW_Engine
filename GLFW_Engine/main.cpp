#define GLEW_STATIC

#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shaderID;

// Vertex Shader
static const char* vertexShader = "					\n\
#version 330										\n\
													\n\
layout (location) in vec3 pos						\n\
													\n\
void main()											\n\
{													\n\
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);	\n\
}";

// Fragment Shader
static const char* fragmentShader = "				\n\
#version 330										\n\
													\n\
out vec4 colour										\n\
													\n\
void main()											\n\
{													\n\
	colour = vec4(0.2, 0.9, 0.9, 1.0);				\n\
}";

GLFWwindow* initMainWindowContext();

void drawTriangle()
{
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void addShader(GLuint shaderProgramID, const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, theCode, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar errorLog[1024] =  { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
		printf("Error compiling %d shader program: %s\n", shaderType, errorLog);
		return;
	}

	glAttachShader(shaderProgramID, shader);
}

void compileShaders()
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		std::cout << "Error creating shader program.\n" << std::endl;
		return;
	}

	addShader(shaderID, vertexShader, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar errorLog[1024] =  { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		printf("Error linking shader program: %s\n", errorLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		printf("Error validating shader program: %s\n", errorLog);
		return;
	}
	
}

int main(int argc, char *args[])
{
	// set up the main window
	GLFWwindow* mainWindow = initMainWindowContext();
	if (!mainWindow)
	{
		std::cout << "Failed to initialise window context! Exiting..." << std::endl;
		return 0;
	}

	// get the frame buffer size
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set the viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	drawTriangle();
	compileShaders();


	while (!glfwWindowShouldClose(mainWindow))
	{
		// get and handle user events
		glfwPollEvents();

		// clear window
		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderID);
			glBindVertexArray(VAO);

				glDrawArrays(GL_TRIANGLES, 0, 3);

			glBindVertexArray(0);		
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	//Quit SDL
	glfwTerminate();
	return 0;
}

GLFWwindow* initMainWindowContext()
{
	std::cout << "Initialising main window context..." << std::endl;

	// start SDL or get Init error
	if (!glfwInit())
	{
		std::cout << "[GLFW] Failed to initialise GLFW!" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	/**** set the SDL OpenGL properties ****/

	// set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// set the Profile Mask to Core profile which will not allow use of deprecated features from previous versions of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/**** Create main window ****/

	// create window centered with defined dimensions
	GLFWwindow* mainWindow = glfwCreateWindow(
		WIDTH,
		HEIGHT,
		"GLFW Test Window",
		NULL,
		NULL
	);

	// quit if windows failed to be created
	if (!mainWindow)
	{
		std::cout << "[GLFW] Unable to create main window. Error: " << std::endl;
		glfwTerminate();
		return nullptr;
	}

	/**** GLEW setup ****/

	// set context in order to draw to main window
	glfwMakeContextCurrent(mainWindow);

	// allow modern extension features
	glewExperimental = GL_TRUE;

	// init GLEW or quit if there is an error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialisation failed!" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return nullptr;
	}

	return mainWindow;
}