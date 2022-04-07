#pragma once
#include "../render/tiles.h"
#include "../render/model.h"
#include "shape.h"

typedef struct map
{
	int colliderCount;
	shape *colliderShapeArray;
	int tileCount;
	tiles *tilesArray;
	int modelCount;
	model *modelArray;
}map;

void InitMap(map *inputMap, char pathToMapFile[]);

void DrawMap(map *inputMap, mat4 *projection, mat4 *view, double currentTime);
