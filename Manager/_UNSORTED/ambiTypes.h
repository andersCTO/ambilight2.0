#pragma once

#include <string>

enum gridType
{
	TV_WALL,
	FLOOR,
	LEFT_WALL,
	RIGHT_WALL
};

struct imPos
{
	uint16_t u = 0;
	uint16_t v = 0;
};
typedef struct imPos imPos;

struct ledPos
{
	float x = 0;
	float y = 0;
	float z = 0;
};
typedef struct ledPos ledPos;

struct gridPos
{
	ledPos ledpos;
	imPos impos;
};


struct nodeCFG
{
	int ipAddr = 0;
	int udpPort = 1234;
	std::string name;
	int ledCount = 0;
	//Image reference vector
	//data pointer
	
};
typedef struct nodeCFG nodeCFG;