#ifndef SEARCH_H
#define SEARCH_H

#include "student.h"

// Sequential search: find by any field, returns count of matches
// matchingIndices must be pre-allocated with at least sys.count elements
int sequentialSearch(DormSystem& sys, int field, const char* keyword, int* matchingIndices);

// Binary search by student ID (array must be sorted by ID first)
// Returns index of match, or -1 if not found
int binarySearchById(DormSystem& sys, const char* id);

// Block search by building number
// Returns index of match, or -1 if not found
int blockSearchByBuilding(DormSystem& sys, const char* building);

// Ensure array is sorted by ID (for binary search)
void ensureSortedById(DormSystem& sys);

// Field constants for sequential search
#define SEARCH_BY_ID       0
#define SEARCH_BY_NAME     1
#define SEARCH_BY_GENDER   2
#define SEARCH_BY_BUILDING 3
#define SEARCH_BY_ROOM     4
#define SEARCH_BY_PHONE    5
#define SEARCH_BY_MAJOR    6

#endif // SEARCH_H
