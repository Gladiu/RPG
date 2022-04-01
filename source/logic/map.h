#pragma once
#include "../render/tiles.h"
#include "../render/background.h"
#include "shape.h"

typedef struct map
{
	background background;
	int colliderCount;
	shape *colliderShapeArray;
	int tileCount;
	tiles *tilesArray;
}map;

void InitMap(map *inputMap, char pathToMapFile[]);

void DrawMap(map *inputMap, mat4 *projection, mat4 *view, double currentTime);
