#include "search.h"
#include <cstring>
#include <cmath>

// Bubble sort by ID (used to ensure sorted order for binary search)
static void sortByStudentId(DormSystem& sys) {
    for (int i = 0; i < sys.count - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < sys.count - 1 - i; j++) {
            if (strcmp(sys.students[j].id, sys.students[j + 1].id) > 0) {
                Student temp = sys.students[j];
                sys.students[j] = sys.students[j + 1];
                sys.students[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

static bool isSortedById(DormSystem& sys) {
    for (int i = 0; i < sys.count - 1; i++) {
        if (strcmp(sys.students[i].id, sys.students[i + 1].id) > 0)
            return false;
    }
    return true;
}

void ensureSortedById(DormSystem& sys) {
    if (!isSortedById(sys)) {
        sortByStudentId(sys);
    }
}

int sequentialSearch(DormSystem& sys, int field, const char* keyword, int* matchingIndices) {
    int matchCount = 0;
    for (int i = 0; i < sys.count; i++) {
        const char* value = NULL;
        switch (field) {
            case SEARCH_BY_ID:       value = sys.students[i].id; break;
            case SEARCH_BY_NAME:     value = sys.students[i].name; break;
            case SEARCH_BY_GENDER:   value = sys.students[i].gender; break;
            case SEARCH_BY_BUILDING: value = sys.students[i].building; break;
            case SEARCH_BY_ROOM:     value = sys.students[i].room; break;
            case SEARCH_BY_PHONE:    value = sys.students[i].phone; break;
            case SEARCH_BY_MAJOR:    value = sys.students[i].major; break;
        }
        if (value && strcmp(value, keyword) == 0) {
            matchingIndices[matchCount++] = i;
        }
    }
    return matchCount;
}

int binarySearchById(DormSystem& sys, const char* id) {
    ensureSortedById(sys);
    int low = 0, high = sys.count - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(sys.students[mid].id, id);
        if (cmp == 0) return mid;
        else if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int blockSearchByBuilding(DormSystem& sys, const char* building) {
    // Sort by building first
    for (int i = 0; i < sys.count - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < sys.count - 1 - i; j++) {
            if (strcmp(sys.students[j].building, sys.students[j + 1].building) > 0) {
                Student temp = sys.students[j];
                sys.students[j] = sys.students[j + 1];
                sys.students[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }

    // Build block index
    int blockSize = 1;
    if (sys.count > 9) blockSize = (int)(sqrt((double)sys.count));
    if (blockSize < 1) blockSize = 1;

    int blockCount = (sys.count + blockSize - 1) / blockSize;

    // Find which block to search
    for (int b = 0; b < blockCount; b++) {
        int start = b * blockSize;
        int end = start + blockSize;
        if (end > sys.count) end = sys.count;

        // Check if this block could contain the building
        if (strcmp(building, sys.students[end - 1].building) <= 0) {
            // Search within this block
            for (int i = start; i < end; i++) {
                if (strcmp(sys.students[i].building, building) == 0) {
                    return i;
                }
            }
        }
    }
    return -1;
}
