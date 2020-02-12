#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "cfgFileManager.hpp"



int getdir(std:: string dir, std::vector<std::string>& files)
{
	std::string tmpString;
	DIR* dp;
	struct dirent* dirp;
	if ((dp = opendir(dir.c_str())) == 0) {
		std::cout << "Error(" << errno << ") opening " << dir << std::endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != 0) {
		if (dirp->d_type == 8) {
			tmpString = dir;
			tmpString.append("/"+ std::string(dirp->d_name));
			files.push_back(tmpString);
		}
			
	}

	


	closedir(dp);
	return 0;
}

int loadCfgFiles(std::vector<std::string>& files) {

	std::string dir = std::string("cfg/positions");
	std::vector<nodeCFG> nodes;
	getdir(dir, files);



	loadNodeCfgs(files, nodes);
	return 0;
}

int loadNodeCfgs(std::vector<std::string>& files, std::vector<nodeCFG>& nodes) {

	std::ifstream myFile;
	std::string myLine,tmpLine;
	int localIp = 0, endpos = 1;
	std::vector<float> ledPos(3);

	nodeCFG nodeTmp;

	for (auto&& f : files) {
		
		//Open file;
		myFile.open(f);
		if (!myFile.is_open())
			continue;
		
		//Load IP adress
		getline(myFile, myLine);
		localIp = 0;
		endpos = 1;
		
		//find 8bit separator
		for (int i = 0; i < 4; i++) {
			if (i < 3) {
				endpos = myLine.find('.');
			}
			else {
				endpos = myLine.find(' ');
			}
			
			if (endpos > 0) {
				tmpLine = myLine.substr(0, endpos);
				myLine = myLine.substr(endpos + 1);
				localIp = stoi(tmpLine) | (localIp << 8);
			}
			else {
				break;
				
			}
		}
		
		nodeTmp.ipAddr = localIp;
		nodeTmp.name = f.substr(f.find_last_of("/") + 1);
		
		//Load port
		getline(myFile, myLine);
		endpos = myLine.find(' ');
		myLine = myLine.substr(0, endpos);
		nodeTmp.udpPort = stoi(myLine);

		getline(myFile, myLine);
		endpos = myLine.find(' ');
		myLine = myLine.substr(0, endpos);
		nodeTmp.ledCount = stoi(myLine);

		//load led positions
		for (int ledPosIdx = 0; ledPosIdx < nodeTmp.ledCount; ledPosIdx++) {
			
			//Get Image reference from grid position
			getline(myFile, myLine);
			for (int col = 0; col < 3; col++) {
				endpos = myLine.find(' ');
				tmpLine = myLine.substr(0, endpos);
				myLine = myLine.substr(endpos + 1);
				ledPos[col] = stof(tmpLine);
			}

			//
			
		}







		//Display node
		std::cout << "Node: " << nodeTmp.name << std::endl;
		std::cout << "\tIP ADDR: " << (unsigned int)nodeTmp.ipAddr << std::endl;
		std::cout << "\tPort: " << (unsigned int)nodeTmp.udpPort << std::endl;
		std::cout << "\tLED Count: " << (unsigned int)nodeTmp.ledCount << std::endl;

		//Append data
		nodes.push_back(nodeTmp);
		
		//Close file		
		myFile.close();

	}
	return 0;
}

