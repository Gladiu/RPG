#include "shape.h"
#include "../libs/cglm/vec2.h"
#include <math.h>

void InitShape(shape* inputShape, vec2 position, float angle, int sidesCount, line* sides[3]){

	inputShape->position[0] = position[0];
	inputShape->position[1] = position[1];
	inputShape->sidesCount = sidesCount;
	inputShape->sides = calloc(1, sizeof(vec2)*sidesCount);
	for(int i = 0; i<sidesCount; i++){
		inputShape->sides[i].start[0] = sides[i]->start[0];
		inputShape->sides[i].start[1] = sides[i]->start[1];
		inputShape->sides[i].end[0] = sides[i]->end[0];
		inputShape->sides[i].end[1] = sides[i]->end[1];
	}
}

void SetPosShape(shape *inputShape, vec2 desiredPosition){
	
	inputShape->position[0] = desiredPosition[0];
	inputShape->position[1] = desiredPosition[1];
}

void SetAngleShape(shape *inputShape, float angle){

	for(int i = 0; i< inputShape->sidesCount; i++){
		//glm_vec2_rotate(inputShape->sides[i], glm_rad(angle), inputShape->sides[i]);
	}
}
bool Collides(shape *shape1, shape* shape2){
	// Implementing Separating Axes Theorem 
	// https://dyn4j.org/2010/01/sat/
	
	for (int i = 0; i < shape1->sidesCount; i++){
		// Calculating normal of each side
		// to use it as an axe
		vec2 p1 = {0.0f, 0.0f};
		vec2 p2 = {0.0f, 0.0f};
		p1[0] = shape1->sides[i].start[0];
		p1[1] = shape1->sides[i].start[1];
		p2[0] = shape1->sides[i].end[0];
		p2[1] = shape1->sides[i].end[1];
		vec2 edge = {p1[0]-p2[0], p1[1]-p1[1]};
		vec2 axe = {-edge[1], edge[0]};
		
		// Projecting shape on axe
		double min = glmc_vec2_dot(axe, shape1->sides[i].start);

	}
	return true;
}
