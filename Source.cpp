#include "Disk.h"
#include "Files.h"
#include "Sectors.h"
#include <iostream>
#include <limits>
#include <cstdlib>
using namespace std;

int main()
{
	Disk Disk1;
	int numOfSectors, sizeOfSectors = 0;

    cout << "Enter number of sectors: ";
    cin >> numOfSectors;

    cout << "\nEnter size of sectors: ";
    cin >> sizeOfSectors;

	Disk1.SetDiskSize(numOfSectors * sizeOfSectors);
	Disk1.SaveFile("test1", 5);
	Disk1.SaveFile("test2", 5);
	Disk1.SaveFile("test3", 5);
	Disk1.PrintDisk();
	cout << endl;
	Disk1.DeleteFile("test2");
	Disk1.PrintDisk();
	cout << endl;
	Disk1.SaveFile("test4", 8);
	Disk1.PrintDisk();
	cout << endl;
	Disk1.Together();
	Disk1.PrintDisk();

	return 0;
}
