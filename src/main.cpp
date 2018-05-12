#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <iomanip>
#include <thread>
#include <math.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "resource_manager.hpp"
#include "game.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main()  
{  
	std::cout << "Hello, world, from MingGW GCC C++!" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Spaceflight Simulator", monitor, NULL);

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glewExperimental = GL_TRUE;
	glewInit();

	// Now in 3D!
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// This is needed to unblock the font rendering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	game.Width = mode->width;
	game.Height = mode->height;
	game.Init();
	int sample = 0;
	auto t_start = std::chrono::high_resolution_clock::now();
	auto t_end = std::chrono::high_resolution_clock::now();
	float deltaTime = 0.0f;

	while(!glfwWindowShouldClose(window) && !game.sigQuit)
	{
		if(sample > 100)
		{
			t_end = std::chrono::high_resolution_clock::now();
			deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(t_end - t_start).count() / (float) sample;
			sample = 0;
			t_start = t_end;
		}
		sample++;
		glfwPollEvents();
		game.ProcessInput(deltaTime);

		game.Update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game.render();
		glfwSwapBuffers(window);
	}

	ResourceManager::Clear();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


// TODO: Add key debouncing with set debounce time (perhaps for toggleable keys only)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			game.pressKey(key);
		}
		else if (action == GLFW_RELEASE)
		{
			game.releaseKey(key);
		}
	}
}