#include "player.h"
#include <GLFW/glfw3.h>

void MovePlayer(int keyPress, player *inputPlayer)
{
	if (keyPress == GLFW_KEY_UP)
		inputPlayer->y += 0.1;
	if (keyPress == GLFW_KEY_DOWN)
		inputPlayer->y -= 0.1;
	if (keyPress == GLFW_KEY_LEFT)
		inputPlayer->x -= 0.1;
	if (keyPress == GLFW_KEY_RIGHT)
		inputPlayer->x += 0.1;
}
