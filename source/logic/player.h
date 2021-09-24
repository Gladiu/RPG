#pragma once

typedef struct player
{
	int x;
	int y;
}player;


// Moves player around based on what GLFW key is being pressed
void MovePlayer(int keyPress, player * inputPlayer);
