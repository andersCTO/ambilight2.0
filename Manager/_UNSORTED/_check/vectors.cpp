#include "stdafx.h"
#include <vector>
#include "space_vector_function.h"




int main()
{
	pos p1, p2;
	p1.x = 2;
	p1.y = 7;
	p2.x = 6;
	p2.y = 2;
	
	vector<pos>pos_vector;

	create_vector(&p1, &p2, 1, pos_vector);

	printf("\nVector size: %u", pos_vector.size());
	for (int i = 0; i < pos_vector.size(); i++) {
		printf("\n (x,y) = (%u,%u)", pos_vector[i].x, pos_vector[i].y);
	}


    return 0;
}

