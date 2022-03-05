#include "shape.h"
//#include "../libs/cglm/vec2.h"
#include <math.h>

static float dot(vec2 a, vec2 b) {
  return a[0] * b[0] + a[1] * b[1];
}
void InitShape(shape* inputShape, vec2 position, float angle, int sidesCount, line sides[sidesCount]){

	inputShape->position[0] = position[0];
	inputShape->position[1] = position[1];
	inputShape->sidesCount = sidesCount;
	inputShape->sides = calloc(1, sizeof(vec2)*sidesCount);
	for(int i = 0; i<sidesCount; i++){
		inputShape->sides[i].start[0] = sides[i].start[0];
		inputShape->sides[i].start[1] = sides[i].start[1];
		inputShape->sides[i].end[0] = sides[i].end[0];
		inputShape->sides[i].end[1] = sides[i].end[1];
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
		vec2 axis = {-edge[1], edge[0]};
		
		// Projecting shape on axe
		typedef struct projection{
			double min;
			double max;
		}projection;
		
		projection projectedShape1;
		projectedShape1.min = dot(axis, shape1->sides[0].start);
		projectedShape1.max = projectedShape1.min;
		for (int i = 1; i < shape1->sidesCount; i++){
			double point = dot(axis, shape1->sides[0].start);
			if ( point < projectedShape1.min){
				projectedShape1.min = point;
			}
			if ( point > projectedShape1.max){
				projectedShape1.max = point;
			}
		}
		projection projectedShape2;
		projectedShape2.min = dot(axis, shape1->sides[0].start);
		projectedShape2.max = projectedShape2.min;
		for (int i = 1; i < shape1->sidesCount; i++){
			double point = dot(axis, shape1->sides[0].start);
			if ( point < projectedShape2.min){
				projectedShape2.min = point;
			}
			if ( point > projectedShape2.max){
				projectedShape2.max = point;
			}
		}
		bool contains = (projectedShape1.min > projectedShape2.min && projectedShape1.min < projectedShape2.max)
			|| (projectedShape1.max > projectedShape2.min && projectedShape1.max < projectedShape2.max  );
		if (!contains){
			return false;
		}
	}

	for (int i = 0; i < shape2->sidesCount; i++){
		// Calculating normal of each side
		// to use it as an axe
		vec2 p1 = {0.0f, 0.0f};
		vec2 p2 = {0.0f, 0.0f};
		p1[0] = shape2->sides[i].start[0];
		p1[1] = shape2->sides[i].start[1];
		p2[0] = shape2->sides[i].end[0];
		p2[1] = shape2->sides[i].end[1];
		vec2 edge = {p1[0]-p2[0], p1[1]-p1[1]};
		vec2 axis = {-edge[1], edge[0]};
		
		// Projecting shape on axe
		typedef struct projection{
			double min;
			double max;
		}projection;
		
		projection projectedShape1;
		projectedShape1.min = dot(axis, shape2->sides[0].start);
		projectedShape1.max = projectedShape1.min;
		for (int i = 1; i < shape2->sidesCount; i++){
			double point = dot(axis, shape2->sides[0].start);
			if ( point < projectedShape1.min){
				projectedShape1.min = point;
			}
			if ( point > projectedShape1.max){
				projectedShape1.max = point;
			}
		}
		projection projectedShape2;
		projectedShape2.min = dot(axis, shape2->sides[0].start);
		projectedShape2.max = projectedShape2.min;
		for (int i = 1; i < shape2->sidesCount; i++){
			double point = dot(axis, shape2->sides[0].start);
			if ( point < projectedShape2.min){
				projectedShape2.min = point;
			}
			if ( point > projectedShape2.max){
				projectedShape2.max = point;
			}
		}
		bool contains = (projectedShape1.min > projectedShape2.min && projectedShape1.min < projectedShape2.max)
			|| (projectedShape1.max > projectedShape2.min && projectedShape1.max < projectedShape2.max  );
		if (!contains){
			return false;
		}

	}
	return true;
}
