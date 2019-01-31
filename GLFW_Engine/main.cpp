#define GLEW_STATIC

#include "pch.h";
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

int main(int argc, char *args[])
{
	std::cout << "Initialising GLFW..." << std::endl;
	system("pause");

	// start SDL or get Init error
	if (!glfwInit())
	{
		std::cout << "[GLFW] Failed to initialise GLFW!" << std::endl;
		glfwTerminate();
		return 1;
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
	GLFWwindow *mainWindow = glfwCreateWindow(
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
		return 1;
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
		return 1;
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
		glClearColor(0.2f, 0.4f, 0.6f, 0.8f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	//Quit SDL
	glfwTerminate();
	return 0;
}