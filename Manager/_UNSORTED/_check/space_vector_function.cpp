#include "stdafx.h"
#include "space_vector_function.h"

void create_vector(pos * p1, pos * p2, double resolution, vector<pos> & pos_vector)
{
	pos point1, point2;

	//Find min x
	if (p1->x < p2->x){
		point1 = *p1;
		point2 = *p2;
	}
	else {
		point1 = *p2;
		point2 = *p1;
	}


	//measure distance between points
	double point_distance = sqrt((double)pow(point1.x - point2.x, 2) + (double)pow(point1.y - point2.y, 2));


	//Allocate memory for vector
	pos_vector.resize((uint16_t)ceil(point_distance * resolution));


	//Calculate x components
	uint16_t vector_index = 0;
	while (vector_index < pos_vector.size()) {
		pos_vector[vector_index].x = point1.x + resolution * vector_index;
		pos_vector[vector_index].y = round((double)(point1.y*(point2.x - pos_vector[vector_index].x) + point2.y*(pos_vector[vector_index].x - point1.x)) / (double)(point2.x - point1.x));
		vector_index++;
	}

	//Calcylcate y components

}
