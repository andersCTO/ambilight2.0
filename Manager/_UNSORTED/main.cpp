#include <iostream>

#include "cfgFileManager.hpp"
#include "gridFunctions.hpp"

using namespace std;
int main(void){

		
		vector<string> files = vector<string>();
		//loadCfgFiles(files);
		

		ledPos start, end, tvPos,room;
		room.x = 3570;
		room.y = 5950;
		room.z = 2400;

		start.x = 3570;
		start.z = 2400;

		end.x = 0;
		end.z = 0;

		tvPos.x = 980;
		tvPos.y = 0;
		tvPos.z = 1120;

		initGridFunctions(640, 480, tvPos,room);
		vector <gridPos> grid;

		createGrid(start, end, grid, TV_WALL);

		return 0;
	
}