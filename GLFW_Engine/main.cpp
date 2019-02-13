#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

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

	while (!glfwWindowShouldClose(mainWindow))
	{
		// get and handle user events
		glfwPollEvents();

		// clear window
		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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