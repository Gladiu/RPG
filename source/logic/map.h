#pragma once
#include "../render/tiles.h"
#include "../render/background.h"
#include "shape.h"

typedef struct map
{
	background background;
	int colliderCount;
	shape *colliderShapeArray;
	tiles *colliderTileArray;
}map;
