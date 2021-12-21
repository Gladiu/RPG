#include "shape.h"

void InitShape(shape inputShape, vec2 position, float angle, int pointCount, vec2 pointsArray[3]){

	inputShape.position[0] = position[0];
	inputShape.position[1] = position[1];
	inputShape.pointCount = pointCount;
	inputShape.pointsArray = calloc(1, sizeof(vec2)*pointCount);
	for(int i = 0; i<pointCount; i++){
		inputShape.pointsArray[i][0] = pointsArray[i][0];
		inputShape.pointsArray[i][1] = pointsArray[i][1];
	}
}
