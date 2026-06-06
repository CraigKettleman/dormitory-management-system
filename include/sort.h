#ifndef SORT_H
#define SORT_H

#include "student.h"

// Bubble sort by student ID (ascending)
void bubbleSortById(DormSystem& sys);

// Quick sort by student name (ascending)
void quickSortByName(DormSystem& sys);

// Insertion sort by building + room (ascending)
void insertionSortByBuildingRoom(DormSystem& sys);

#endif // SORT_H
