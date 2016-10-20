#ifndef FILES_H
#define FILES_H
#include "Sectors.h"
#include <string>
struct Files{
	std::string Name;
	int NumChars;
	int DataNum;
	Sectors* nextSector;
	Files* next;
};
#endif // FILES_H
