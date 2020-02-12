#include <iostream>
#include <math.h>
#include "gridFunctions.hpp"


static int imageWidth = -1;
static int imageHeight = -1;
static ledPos TVcenterPos;
static ledPos RoomSize;




int initGridFunctions(int iWidth,int iHeight,ledPos tvPos,ledPos room) {

	imageWidth = iWidth;
	imageHeight = iHeight;
	TVcenterPos = tvPos;
	RoomSize = room;

	return 0;
}


int createGrid(ledPos start, ledPos end, std::vector<gridPos>& grid, gridType type) {

	ledPos currentPos;
	gridPos gridCel;
	int	leftSquareWidth, rightSquareWidth, topSquareHeight, bottomSquareHeight;
	float leftSquareRes,rightSquareRes,topSquareRes,bottomSquareRes;
	
	float uSpace, vSpace,uPos,vPos; //(u= width, v=height)

	float startX, startY, startZ;
	float endX, endY, endZ;

	switch (type)
	{
	case TV_WALL:

		
		//Create the four squares
		
		leftSquareWidth = (RoomSize.x - TVcenterPos.x) / RoomSize.x * imageWidth;//pixels
		rightSquareWidth = imageWidth - leftSquareWidth;//pixels

		leftSquareRes = (RoomSize.x - TVcenterPos.x) / (float)leftSquareWidth;//mm per pixel
		rightSquareRes = TVcenterPos.x / (float)rightSquareWidth;//mm per pixel


		topSquareHeight = (RoomSize.z - TVcenterPos.z) / RoomSize.z * imageHeight;
		bottomSquareHeight = imageHeight - topSquareHeight;

		topSquareRes = (RoomSize.z - TVcenterPos.z) / (float)topSquareHeight;//mm per pixel
		bottomSquareRes = TVcenterPos.z / (float)bottomSquareHeight;//mm per pixel

		std::cout << "Creating grid for TV Wall with: " << std::endl;
		std::cout << "\tleftSquareWidth: " << leftSquareWidth << std::endl;
		std::cout << "\trightSquareWidth: " << rightSquareWidth <<  std::endl;

		std::cout << "\tleftSquareRes: " << leftSquareRes << std::endl;
		std::cout << "\trightSquareRes: " << rightSquareRes << std::endl;

		std::cout << "\ttopSquareHeight: " << topSquareHeight << std::endl;
		std::cout << "\tbottomSquareHeight: " << bottomSquareHeight << std::endl;
		
		std::cout << "\ttopSquareRes: " << topSquareRes << std::endl;
		std::cout << "\tbottomSquareRes: " << bottomSquareRes << std::endl;


		uPos = start.x;
		vPos = start.z;
		for (int iRow = 0; iRow < imageHeight; iRow++) {

			for (int iCol = 0; iCol < imageWidth; iCol++) {
				
				gridCel.impos.u = iCol;
				gridCel.impos.v = iRow;
				gridCel.ledpos.x = uPos;
				gridCel.ledpos.z = vPos;
				gridCel.ledpos.y = 0;
				grid.push_back(gridCel);


				if (iCol < leftSquareWidth) {//Left side
					uPos -= leftSquareRes;

				}
				else {//right side
					uPos -= rightSquareRes;
				}			
			}
			uPos = start.x;
			if (iRow < topSquareHeight) {//Top side
				vPos -= topSquareRes;

			}
			else {//bottom side
				vPos -= bottomSquareRes;
			}
		}

		std::cout << "\tGrid of size: " << grid.size()<< std::endl;
		break;
	case FLOOR:
		std::cout << "Creating floor grid" << std::endl;
		//Create two grids
		leftSquareWidth = (RoomSize.x - TVcenterPos.x) / RoomSize.x * imageWidth;//pixels
		rightSquareWidth = imageWidth - leftSquareWidth;//pixels

		leftSquareRes = (RoomSize.x - TVcenterPos.x) / (float)leftSquareWidth;//mm per pixel
		rightSquareRes = TVcenterPos.x / (float)rightSquareWidth;//mm per pixel

		topSquareRes = 


		start.z = 0;
		end.z = 0;
		break;
	default:
		std::cout << "Bad grid type"<<std::endl;
		return -1;
		break;
	}



}