#include "shape.h"
//#include "../libs/cglm/vec2.h"
#include <limits.h> // DBL_MAX
#include <math.h>

float dot(vec2 a, vec2 b) {
  return a[0] * b[0] + a[1] * b[1];
}
void InitShape(shape* inputShape, vec2 position, float angle, int sidesCount, line sides[sidesCount]){

	inputShape->position[0] = position[0];
	inputShape->position[1] = position[1];
	inputShape->sidesCount = sidesCount;
	inputShape->sides = calloc(1, sizeof(line)*sidesCount);
	for(int i = 0; i<sidesCount; i++){
		inputShape->sides[i].start[0] = sides[i].start[0];
		inputShape->sides[i].start[1] = sides[i].start[1];
		inputShape->sides[i].end[0] = sides[i].end[0];
		inputShape->sides[i].end[1] = sides[i].end[1];
	}
}

void DestroyShape(shape* inputShape){
	free(inputShape->sides);
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
bool Collides(shape *shape1, shape* shape2, vec2* MTV){
	// Implementing Separating Axes Theorem 
	// https://dyn4j.org/2010/01/sat/
	
	// Smallest overlap for MTV calculation
	double smallestOverlap = DBL_MAX;
	for (int i = 0; i < shape1->sidesCount; i++){


		// Calculating normal of each side
		// to use it as an axe
		vec2 p1 = {0.0f, 0.0f};
		vec2 p2 = {0.0f, 0.0f};

		p1[0] = shape1->sides[i].start[0] + shape1->position[0];
		p1[1] = shape1->sides[i].start[1] + shape1->position[1];
		p2[0] = shape1->sides[i].end[0] + shape1->position[0];
		p2[1] = shape1->sides[i].end[1] + shape1->position[1];
		vec2 edge = {p1[0]-p2[0], p1[1]-p2[1]};
		vec2 axis = {-edge[1], edge[0]};
		
		// Normalising axis is needed for finding Minimal Translation Vector
		// that pushes collided objects away from each other
		double axisLength = sqrt(axis[0]*axis[0] + axis[1]*axis[1]);
		axis[0] /= axisLength;
		axis[1] /= axisLength;
		
		// Projecting shape on 1D axis
		typedef struct projection{
			double min;
			double max;
		}projection;
		
		projection projectedShape1;
		projectedShape1.min = dot(axis,(vec2){shape1->sides[i].start[0] + shape1->position[0],
				shape1->sides[i].start[1] + shape1->position[1]} );
		projectedShape1.max = projectedShape1.min;
		for (int i = 1; i < shape1->sidesCount; i++){
			double point = dot(axis,(vec2){shape1->sides[i].start[0] + shape1->position[0],
					shape1->sides[i].start[1] + shape1->position[1]} );
			if ( point < projectedShape1.min){
				projectedShape1.min = point;
			}
			else if ( point > projectedShape1.max){
				projectedShape1.max = point;
			}
		}
		projection projectedShape2;
		projectedShape2.min = dot(axis, shape2->sides[0].start);
		projectedShape2.max = projectedShape2.min;
		for (int i = 1; i < shape2->sidesCount; i++){
			double point = dot(axis,(vec2){shape2->sides[i].start[0] + shape2->position[0],
					shape2->sides[i].start[1] + shape2->position[1]} );
			if ( point < projectedShape2.min){
				projectedShape2.min = point;
			}
			else if ( point > projectedShape2.max){
				projectedShape2.max = point;
			}
		}
		bool twoContainsOne = ((projectedShape1.min >= projectedShape2.min && projectedShape1.min <= projectedShape2.max)
			|| (projectedShape1.max >= projectedShape2.min && projectedShape1.max <= projectedShape2.max  ));
		bool oneContainsTwo = ((projectedShape2.min >= projectedShape1.min && projectedShape2.min <= projectedShape1.max)
			|| (projectedShape2.max >= projectedShape1.min && projectedShape2.max <= projectedShape1.max  ));
		if (!(oneContainsTwo || twoContainsOne)){
			(*MTV)[0] = 0;
			(*MTV)[1] = 0;
			return false;
		}
		else{
			// Calculating MTV
			// we are looking for smallest overlap
			// and then save the axis for this smallest overlap
			//
			// Overlap is distance between biggest min and smallest max
			double min;
			if ( projectedShape1.min > projectedShape2.min ){
				min = projectedShape1.min;
			}
			else{
				min = projectedShape2.min;
			}
			double max;
			if ( projectedShape1.max > projectedShape2.max ){
				max = projectedShape1.max;
			}
			else{
				max = projectedShape2.max;
			}
			
			double overlap = fabs(max-min);
			if (overlap < smallestOverlap){
				smallestOverlap = overlap;
	fprintf(stderr, "axis[0] = %f, axis[1] = %f \n", axis[0], axis[1]);
				(*MTV)[0] = axis[0];
				(*MTV)[1] = axis[1];
			}
		}
	}
	for (int i = 0; i < shape2->sidesCount; i++){
		// Calculating normal of each side
		// to use it as an axe
		vec2 p1 = {0.0f, 0.0f};
		vec2 p2 = {0.0f, 0.0f};
		p1[0] = shape2->sides[i].start[0] + shape2->position[0];
		p1[1] = shape2->sides[i].start[1] + shape2->position[1];
		p2[0] = shape2->sides[i].end[0] + shape2->position[0];
		p2[1] = shape2->sides[i].end[1] + shape2->position[1];
		vec2 edge = {p1[0]-p2[0], p1[1]-p2[1]};
		vec2 axis = {-edge[1], edge[0]};
		
		// Projecting shape on axis
		typedef struct projection{
			double min;
			double max;
		}projection;
		// Normalising axis is needed for finding Minimal Translation Vector
		// that pushes collided objects away from each other
		double axisLength = sqrt(axis[0]*axis[0] + axis[1]*axis[1]);
		axis[0] /= axisLength;
		axis[1] /= axisLength;
		
		
		projection projectedShape1;
		projectedShape1.min = dot(axis,(vec2){shape1->sides[i].start[0] + shape1->position[0],
				shape1->sides[i].start[1] + shape1->position[1]} );
		projectedShape1.max = projectedShape1.min;
		for (int i = 1; i < shape1->sidesCount; i++){
			double point = dot(axis,(vec2){shape1->sides[i].start[0] + shape1->position[0],
					shape1->sides[i].start[1] + shape1->position[1]} );
			if ( point < projectedShape1.min){
				projectedShape1.min = point;
			}
			else if ( point > projectedShape1.max){
				projectedShape1.max = point;
			}
		}
		projection projectedShape2;
		projectedShape2.min = dot(axis, shape2->sides[0].start);
		projectedShape2.max = projectedShape2.min;
		for (int i = 1; i < shape2->sidesCount; i++){
			double point = dot(axis,(vec2){shape2->sides[i].start[0] + shape2->position[0],
					shape2->sides[i].start[1] + shape2->position[1]} );
			if ( point < projectedShape2.min){
				projectedShape2.min = point;
			}
			if ( point > projectedShape2.max){
				projectedShape2.max = point;
			}
		}

		bool twoContainsOne = ((projectedShape1.min >= projectedShape2.min && projectedShape1.min <= projectedShape2.max)
			|| (projectedShape1.max >= projectedShape2.min && projectedShape1.max <= projectedShape2.max  ));
		bool oneContainsTwo = ((projectedShape2.min >= projectedShape1.min && projectedShape2.min <= projectedShape1.max)
			|| (projectedShape2.max >= projectedShape1.min && projectedShape2.max <= projectedShape1.max  ));
		if (!(oneContainsTwo || twoContainsOne)){
			(*MTV)[0] = axis[0];
			(*MTV)[1] = axis[1];
			return false;
		}
		else{
			// Calculating MTV
			// we are looking for smallest overlap
			// and then save the axis for this smallest overlap
			//
			// Overlap is distance between biggest min and smallest max
			double min;
			if ( projectedShape1.min > projectedShape2.min ){
				min = projectedShape1.min;
			}
			else{
				min = projectedShape2.min;
			}
			double max;
			if ( projectedShape1.max > projectedShape2.max ){
				max = projectedShape1.max;
			}
			else{
				max = projectedShape2.max;
			}
			
			double overlap = fabs(max-min);
			if (overlap < smallestOverlap){
				smallestOverlap = overlap;

	fprintf(stderr, "axis[0] = %f, axis[1] = %f \n", axis[0], axis[1]);
				(*MTV)[0] = axis[0];
				(*MTV)[1] = axis[1];
			}
		}

	}
	fprintf(stderr, "MTV[0] = %f, MTV[1] = %f \n", *MTV[0], *MTV[1]);
	return true;
}

void IfWillCollideTrimSpeed(shape* inputShape, vec2 *speed, int shapeCount, shape* shapeArray[shapeCount]){
	for (int i = 0; i < shapeCount; i++){

		// Entire point of this function is not to use MTV
		Collides(inputShape, shapeArray[i], &(vec2){0.0f, 0.0f});
	}
}
