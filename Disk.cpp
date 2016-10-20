#include "Disk.h"


Disk::Disk() : DataNum(1), NumFiles(0), PoolSpace(0)
{
    headFile = NULL;
}


Disk::~Disk()
{
}


void Disk::DeleteFile(string FileName)
{
	tempFile = headFile;

	//Finds the file to be deleted
	while (tempFile->Name != FileName)
	{
		prevFile = tempFile;
		tempFile = tempFile->next;
	}

	//Loop to go through all the sectors held by the file and delete them
	do
	{
		tempSector = deleteSector = tempFile->nextSector;

		if (tempSector->next == NULL)
		{
			tempFile->nextSector = NULL;
		}
		else
		{
			tempSector = tempSector->next;
			tempFile->nextSector = tempSector;
		}

		deleteSector->next = NULL;

		RemoveDataOnDisk(deleteSector);

	} while (tempFile->nextSector != NULL);


	//If-else statements to delete the file
	if (headFile == tempFile)
	{
		headFile = headFile->next;
	}
	else if (tempFile->next == NULL)
	{
		prevFile->next = NULL;
	}
	else
	{
		prevFile->next = tempFile->next;
	}

	delete tempFile;

	NumFiles--;
}


void Disk::PrintDisk() const
{
	for (int i = 0; i < Disk_V.size(); i++)
	{
		cout << i << ": " << Disk_V.at(i) << endl;
	}
}


void Disk::RemoveDataOnDisk(Sectors* putBack)
{
	tempPool = headPool;

	//Loop for putting the data back in the pool based on the sectors left in the pool
	do
	{
		if (putBack->LowRange > tempPool->HighRange &&
			(tempPool->HighRange + 1) != putBack->LowRange)
		{
			prevPool = tempPool;
			tempPool = tempPool->next;
		}
		else if (putBack->LowRange > tempPool->HighRange &&
			(tempPool->HighRange + 1) == putBack->LowRange &&
			(tempPool->next->LowRange - 1) == putBack->HighRange &&
			tempPool == headPool)
		{
			tempPool->HighRange = tempPool->next->HighRange;
			headPool->next = tempPool->next->next;
			prevPool = NULL;
		}
		else if (putBack->LowRange > tempPool->HighRange &&
			(tempPool->HighRange + 1) == putBack->LowRange &&
			(tempPool->next->LowRange - 1) == putBack->HighRange)
		{
			tempPool->next->LowRange = tempPool->LowRange;
			tempPool = NULL;
		}
		else if (putBack->LowRange > tempPool->HighRange &&
			(tempPool->HighRange + 1) == putBack->LowRange)
		{
			tempPool->HighRange = putBack->HighRange;
			prevPool = NULL;
		}
		else if (putBack->LowRange < tempPool->HighRange &&
			putBack->HighRange == (tempPool->LowRange - 1) &&
			tempPool == headPool)
		{
			tempPool->LowRange = putBack->LowRange;
			prevPool = NULL;
		}
		else if (putBack->LowRange < tempPool->HighRange &&
			putBack->HighRange != (tempPool->LowRange - 1) &&
			tempPool == headPool)
		{
			Pool = new Sectors;
			Pool->LowRange = putBack->LowRange;
			Pool->HighRange = putBack->HighRange;
			headPool = Pool;
			headPool->next = tempPool;
			prevPool = NULL;
		}
		else
		{
			Pool = new Sectors;
			Pool->LowRange = putBack->LowRange;
			Pool->HighRange = putBack->HighRange;
			Pool->next = tempPool;
			tempPool = prevPool;
			tempPool->next = Pool;
			prevPool = NULL;
		}
	} while (prevPool != NULL);

	//Clears the data from the disk
	for (int i = putBack->LowRange; i <= putBack->HighRange; i++)
	{
		Disk_V.at(i) = 0;
	}

	delete putBack;
}


bool Disk::SaveFile(string FileName, int NumChars)
{
	//Keeps track of the available space left on the disk
	for (int i = 0; i < Disk_V.size(); i++)
	{
		if (Disk_V.at(i) == 0)
		{
			PoolSpace++;
		}
	}

	//Gives us an error if you try and make a file and do not have enough space left on the
	//disk to make it
	if (PoolSpace < NumChars)
	{
		cout << "Error, cannot save file! Not enough space!\n";
		return false;
	}

	//If-else statement for creating new files
	if (headFile == NULL)
	{
		File = new Files;
		File->Name = FileName;
		File->NumChars = NumChars;
		File->DataNum = this->DataNum;
		tempFile = File;
		headFile = File;
		tempFile->next = NULL;
		tempFile->nextSector = NULL;
	}
	else
	{
		tempFile = headFile;

		while (tempFile->next != NULL)
		{
			tempFile = tempFile->next;
		}

		File = new Files;
		File->Name = FileName;
		File->NumChars = NumChars;
		File->DataNum = this->DataNum;
		tempFile->next = File;
		tempFile = tempFile->next;
		tempFile->next = NULL;
		tempFile->nextSector = NULL;
	}

	SetDataOnDisk(NumChars, File->DataNum);
	DataNum++;
	NumFiles++;

	return true;
}


void Disk::SetDataOnDisk(int Data, int DataNum)
{
	tempPool = headPool;

	//Loop for setting data on the disk and removing the space from the pool
	do
	{
		if ((tempPool->HighRange - tempPool->LowRange + 1) == Data)
		{
			int i = tempPool->LowRange;

			do
			{
				Disk_V.at(i) = DataNum;
				i++;
				Data--;
			} while (i <= tempPool->HighRange && Data != 0);

			if (tempFile->nextSector == NULL)
			{
				Sector = new Sectors;
				Sector->LowRange = tempPool->LowRange;
				Sector->HighRange = tempPool->HighRange;
				tempFile->nextSector = Sector;
				tempSector = Sector;
				tempSector->next = NULL;
			}
			else
			{
				Sector = new Sectors;
				Sector->LowRange = tempPool->LowRange;
				Sector->HighRange = tempPool->HighRange;
				tempSector->next = Sector;
				tempSector = tempSector->next;
				tempSector->next = NULL;
			}

			headPool = headPool->next;

		}
		else if ((tempPool->HighRange - tempPool->LowRange + 1) > Data)
		{
			int i = tempPool->LowRange;

			do
			{
				Disk_V.at(i) = DataNum;
				i++;
				Data--;
			} while (i <= tempPool->HighRange && Data != 0);

			if (tempFile->nextSector == NULL)
			{
				Sector = new Sectors;
				Sector->LowRange = tempPool->LowRange;
				Sector->HighRange = i - 1;
				tempFile->nextSector = Sector;
				tempSector = Sector;
				tempSector->next = NULL;
			}
			else
			{
				Sector = new Sectors;
				Sector->LowRange = tempPool->LowRange;
				Sector->HighRange = i - 1;
				tempSector->next = Sector;
				tempSector = tempSector->next;
				tempSector->next = NULL;
			}

			tempPool->LowRange = i;
		}
		else if ((tempPool->HighRange - tempPool->LowRange + 1) < Data)
		{
			int i = tempPool->LowRange;

			do
			{
				Disk_V.at(i) = DataNum;
				i++;
				Data--;
			} while (i <= tempPool->HighRange && Data != 0);

			if (tempFile->nextSector == NULL)
			{
				Sector = new Sectors;
				Sector->LowRange = tempPool->LowRange;
				Sector->HighRange = tempPool->HighRange;
				tempFile->nextSector = Sector;
				tempSector = Sector;
				tempSector->next = NULL;
			}
			else
			{
				Sector = new Sectors;
				Sector->LowRange = tempPool->LowRange;
				Sector->HighRange = tempPool->HighRange;
				tempSector->next = Sector;
				tempSector = tempSector->next;
				tempSector->next = NULL;
			}

			headPool = headPool->next;
		}

	 	tempPool = tempPool->next;
	} while (Data != 0);
}


void Disk::SetDiskSize(int Size)
{
	Disk_V.resize(Size);

	Pool = new Sectors;
	Pool->LowRange = 0;
	Pool->HighRange = Size - 1;
	tempPool = Pool;
	headPool = Pool;
	tempPool->next = NULL;
}


void Disk::Together()
{
	tempFile = headFile;

	//This loop puts back all the data from the files back into the pool
	for (int i = 0; i < NumFiles; i++)
	{
		do
		{
			tempSector = deleteSector = tempFile->nextSector;

			if (tempSector->next == NULL)
			{
				tempFile->nextSector = NULL;
			}
			else
			{
				tempSector = tempSector->next;
				tempFile->nextSector = tempSector;
			}

			deleteSector->next = NULL;

			RemoveDataOnDisk(deleteSector);

		} while (tempFile->nextSector != NULL);

		tempFile = tempFile->next;
	}


	tempFile = headFile;

	//This loop adds the files back onto the disk
	for (int i = 0; i < NumFiles; i++)
	{
		SetDataOnDisk(tempFile->NumChars, tempFile->DataNum);

		tempFile = tempFile->next;
	}
}


void Disk::Display()
{
	int i = 0;
	tempFile = headFile;

	do
	{
		cout << endl << tempFile->Name << endl;
		i = 0;
		tempSector = tempFile->nextSector;
		do
		{
			cout << "Sector" << i << ": " << tempSector->LowRange << " to " << tempSector->HighRange << endl;
			i++;
			tempSector = tempSector->next;
		} while (tempSector != NULL);
		tempFile = tempFile->next;
	} while (tempFile != NULL);

	tempPool = headPool;
	do
	{
		cout << endl << "Pool: " << endl;
		i = 0;
		cout << "Sector" << i << ": " << tempPool->LowRange << " to " << tempPool->HighRange << endl;
		tempPool = tempPool->next;
	} while (tempPool != NULL);

	cout << "\nDisk: \n";
	for (int i = 0; i < Disk_V.size(); i++)
	{
		cout << i << ": " << Disk_V.at(i) << endl;
	}
}
