#include "libs/cglm/cam.h"
#include "stdio.h"
#include "stdlib.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "libs/cglm/call.h"
#include "render/tiles.h"
#include "render/sprites.h"

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

typedef struct Vector2
{
	int x;
	int y;
}Vector2;



int main()
{
	// Setting main window size
	struct Vector2 windowSize;
	windowSize.x = 800;
	windowSize.y = 600;
	// Setting up glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *mainWindow = glfwCreateWindow(windowSize.x, windowSize.y,"Platform v0.01", NULL, NULL );
	glfwSetWindowAttrib(mainWindow, GLFW_FLOATING, GLFW_TRUE);
	glfwSetWindowSize(mainWindow, windowSize.x, windowSize.y);
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
	glEnable              ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( MessageCallback, 0 );
	
	int map[16]=
	{
		1,1,1,1,
		1,1,1,1,
		1,1,1,1,
		1,1,1,1
	};

	tiles *tilePtr = malloc(sizeof(tiles));
	sprites *spritePtr = malloc(sizeof(sprites));

	// Assiging all of the matrices of sprite and tiles
	glm_perspective(75, 4.0f/3.0f, 0.1f, 100.0f, tilePtr->projection);
	glm_perspective(75, 4.0f/3.0f, 0.1f, 100.0f, spritePtr->projection);
	glm_mat4_identity(tilePtr->view);
	glm_mat4_identity(spritePtr->view);
	glm_mat4_identity(tilePtr->model);
	glm_mat4_identity(spritePtr->model);
	vec4 eye = {0.0, 0.0, -5.0};
	vec4 target = {0.0, 0.0, 0.0};
	vec4 up = {0.0, 1.0, 0.0};
	glm_lookat( eye, target, up, tilePtr->view); 
	glm_lookat( eye, target, up, spritePtr->view); 
	InitTiles(tilePtr, map, 4, 4);
	InitSprites(spritePtr);
	// Setting main game loop
	while(!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();
		// Setting clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Handling input
		vec4 move = {0.0, 0.0, 0.0, 0.0};
		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_W))
			move[1] = move[1]+0.1;
		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_S))
			move[1] = move[1]-0.1;
		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_A))
			move[0] = move[0]+0.1;
		if (GLFW_PRESS == glfwGetKey(mainWindow, GLFW_KEY_D))
			move[0] = move[0]-0.1;
		// Debug
		glm_translate(tilePtr->view, move);
		glm_translate(spritePtr->view, move);
		glm_translate(spritePtr->model, move);
		// All draw calls should be issued here
		DrawTiles(tilePtr);
		DrawSprites(spritePtr);
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}


