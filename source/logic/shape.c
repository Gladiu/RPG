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

void SetPosShape(shape *inputShape, vec2 desiredPosition){
	
	inputShape->position[0] = desiredPosition[0];
	inputShape->position[1] = desiredPosition[1];
}

void SetAngleShape(shape *inputShape, float angle){

	for(int i = 0; i< inputShape->pointCount; i++){
		glmc_vec2_rotate(inputShape->pointsArray[i], glm_rad(angle), inputShape->pointsArray[i]);
	}
}
bool  Collides(shape *shape1, shape shape2){
	// xd
	return true;
}

