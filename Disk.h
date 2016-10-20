#ifndef DISK_H
#define DISK_H
#include "Files.h"
#include "Sectors.h"
#include <iostream>
#include <vector>
using namespace std;
class Disk
{
	friend class Pool;
public:
	Disk(); //Default constructor
	~Disk(); //Default destructor
	bool SaveFile(string FileName, int NumChars); //Funtion to save a file by taking in a name and number of bits or "characters"
	void DeleteFile(string FileName); //Function to delete a file based on its name
	void PrintDisk() const; //Prints the contents of the disk
	void RemoveDataOnDisk(Sectors* putBack); //Removes the data on the disk and puts it back into the pool
	void SetDataOnDisk(int Data, int DataNum); //Funtion to set data on the disk when you save a file
	void SetDiskSize(int Size); //Sets the size of the disk
	void Together(); //Function to ajdust the data on the disk so the data for each files is continuous
	void Display();
private:
	Files* headFile; //File ptr to hold the first file in the list
	Files* prevFile; //File ptr to hold the file prev of the temp
	Files* tempFile; //File ptr to hold a temp File
	Files* File; //Generic name to create a file
	Sectors* deleteSector;  //Sector ptr to hold a sector for deleting
	Sectors* headPool; //Pool ptr to hold the first pool in the list
	Sectors* Pool; //Generic name to create a pool
	Sectors* prevPool; //Pool ptr to hold the pool prev of the temp
	Sectors* Sector; //Generic name to create a sector
	Sectors* tempPool; //Pool ptr to hold a temp pool
	Sectors* tempSector; //Sector ptr to hold a temp sector
	int DataNum; //Holds the data value for a file
	int NumFiles; //Holds the numbers of files
	int PoolSpace; //Holds the number of available
	vector<int> Disk_V; //Holds the data for the disk
};
#endif // DISK_H
