//#pragma once
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

#include "ambiTypes.h"



int getdir(std::string dir, std::vector<std::string>& files);
int loadCfgFiles(std::vector<std::string>& files);
int loadNodeCfgs(std::vector<std::string>& files, std::vector<nodeCFG>& nodes);
