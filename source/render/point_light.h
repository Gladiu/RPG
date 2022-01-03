#pragma once
#include <GLFW/glfw3.h>
#include "../libs/cglm/cglm.h"

// Directional lighthing

typedef struct point_light
{
	vec3 position;
	vec3 color;
	float strength;
}point_light;
