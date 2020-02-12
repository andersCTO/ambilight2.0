
#include "ambiTypes.h"
#include <vector>


int initGridFunctions(int iWidth, int iHeight, ledPos tvPos, ledPos room);
int createGrid(ledPos start,ledPos end,std::vector<gridPos>& grid, gridType type);
