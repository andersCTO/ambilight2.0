#pragma once

#include <vector>

using  namespace std;


struct pos {
	int16_t x;
	int16_t y;
};

typedef struct pos pos;

void create_vector(pos * p1, pos * p2, double resolution, vector<pos> & pos_vector);//Create vector in centimeter resolution