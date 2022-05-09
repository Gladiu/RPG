#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "libs/cglm/include/cglm/cglm.h"
#include "render/tiles.h"
#include "render/point_light.h"
#include "render/sprite.h"
#include "logic/player.h"
#include "logic/destructible.h"
#include "logic/map.h"

// Debug includes
#include <inttypes.h>

#define WINDOW_HEIGHT 600.0f
#define WINDOW_WIDTH 800.0f

// Defining various callbacks

// Function that gets executed each time there is a keystroke happening
void KeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE); 
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}


// Get time in seconds with us precision
double GetTime()
{
	return (double)(glfwGetTimerValue())/1000000000.0;
}

int main()
{

	// Setting up glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,"Platform v0.01", NULL, NULL );
	glfwSetWindowAttrib(mainWindow, GLFW_FLOATING, GLFW_TRUE);
	glfwSetWindowSize(mainWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwRestoreWindow(mainWindow);
	fprintf(stderr, "Maximised window %d \n", glfwGetWindowAttrib(mainWindow, GLFW_MAXIMIZED));

	if (mainWindow == NULL)
	{
		fprintf(stderr, "Failed to create GLFWwindow! Aborting. \n");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(mainWindow);

	// Setting key callback function
	glfwSetKeyCallback(mainWindow, KeyCallback);

	// Setting up glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize glew! Aborting. \n");
		exit(EXIT_FAILURE);
	}

	// During init, enable debug output
	glEnable ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( MessageCallback, 0 );
	
	glEnable(GL_DEPTH_TEST);


	/*
	 *
	 * Initializing Game variables
	 *
	 */
	map *testMap = calloc(1, sizeof(map));
	InitMap(testMap, "../source/map/test.json");

	mat4 generalProjection;
	glm_mat4_identity(generalProjection);
	//glm_rotate_y(generalProjection, glm_rad(180.0f), generalProjection);
	glm_perspective(glm_rad(55.0f), WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f , 100.0f, generalProjection);

	player *mainPlayer = calloc(1, sizeof(player));
	InitPlayer(mainPlayer, &generalProjection);

	// Initializing variables to keep track of time
	double nowTime = 0;
	double lastTime = GetTime(); // lastTime is current time here to not make deltaTime equal to nowTime at first iteration
	double deltaTime = 0;

	// Setting main game loop
	while(!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();
		nowTime = GetTime();
		deltaTime = nowTime-lastTime;
		lastTime = nowTime;

		// Setting clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Gray color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handling input
		// Magic values are temporary for testing
		vec2 move = {0.0, 0.0};
		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_W))
			move[1] = move[1]-1;
		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_S))
			move[1] = move[1]+1;
		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_A))
			move[0] = move[0]-1;
		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_D))
			move[0] = move[0]+1;

		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_E)){
			glm_rotate( mainPlayer->view , 2.0f*deltaTime, (vec4){1.0f, 0.0f, 0.0f, 0.0f} );
		}
		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_Q)){
			glm_rotate( mainPlayer->view , 2.0f*deltaTime, (vec4){0.0f, 1.0f, 0.0f, 0.0f} );
		}

		
		CalculateVelocity(mainPlayer,move, deltaTime, 1);

		// DEBUG
		
		//shape *shapeArray[1] = {&crate->collisionShape};
		//IfWillCollideTrimSpeed(&mainPlayer->collisionShape, &mainPlayer->velocity, 1, shapeArray);
		
		ApplyVelocity(mainPlayer);
		
		DrawMap(testMap, &generalProjection, &(mainPlayer->view), nowTime);
		// All draw calls should be issued here
		DrawPlayer(mainPlayer, nowTime);
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}


