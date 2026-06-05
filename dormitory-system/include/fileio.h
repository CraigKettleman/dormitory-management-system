#ifndef FILEIO_H
#define FILEIO_H

#include "student.h"

// Initialize DormSystem with empty array
void initSystem(DormSystem& sys);

// Load records from data.txt, returns number loaded
int loadFromFile(DormSystem& sys, const char* filename);

// Save all records to data.txt, returns number saved
int saveToFile(DormSystem& sys, const char* filename);

// Free allocated memory
void destroySystem(DormSystem& sys);

#endif // FILEIO_H
