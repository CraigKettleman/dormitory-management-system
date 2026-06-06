#include "sort.h"
#include <cstring>

// ========== Bubble Sort by ID ==========
void bubbleSortById(DormSystem& sys) {
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

// ========== Quick Sort by Name ==========
static int partitionByName(Student* arr, int low, int high) {
    // Median-of-three pivot selection
    int mid = (low + high) / 2;
    if (strcmp(arr[low].name, arr[mid].name) > 0) {
        Student t = arr[low]; arr[low] = arr[mid]; arr[mid] = t;
    }
    if (strcmp(arr[low].name, arr[high].name) > 0) {
        Student t = arr[low]; arr[low] = arr[high]; arr[high] = t;
    }
    if (strcmp(arr[mid].name, arr[high].name) > 0) {
        Student t = arr[mid]; arr[mid] = arr[high]; arr[high] = t;
    }
    // Place pivot at high-1
    Student t = arr[mid]; arr[mid] = arr[high - 1]; arr[high - 1] = t;

    Student pivot = arr[high - 1];
    int i = low, j = high - 1;
    while (true) {
        while (strcmp(arr[++i].name, pivot.name) < 0) {}
        while (strcmp(arr[--j].name, pivot.name) > 0) {}
        if (i < j) {
            Student tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        } else break;
    }
    // Restore pivot
    Student tmp = arr[i]; arr[i] = arr[high - 1]; arr[high - 1] = tmp;
    return i;
}

static void quickSortByNameHelper(Student* arr, int low, int high) {
    if (low + 10 <= high) {
        int pivotIdx = partitionByName(arr, low, high);
        quickSortByNameHelper(arr, low, pivotIdx - 1);
        quickSortByNameHelper(arr, pivotIdx + 1, high);
    } else {
        // Use insertion sort for small subarrays
        for (int i = low + 1; i <= high; i++) {
            Student key = arr[i];
            int j = i - 1;
            while (j >= low && strcmp(arr[j].name, key.name) > 0) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }
}

void quickSortByName(DormSystem& sys) {
    if (sys.count > 1) {
        quickSortByNameHelper(sys.students, 0, sys.count - 1);
    }
}

// ========== Insertion Sort by Building + Room ==========
static int compareBuildingRoom(const Student& a, const Student& b) {
    int cmp = strcmp(a.building, b.building);
    if (cmp != 0) return cmp;
    return strcmp(a.room, b.room);
}

void insertionSortByBuildingRoom(DormSystem& sys) {
    for (int i = 1; i < sys.count; i++) {
        Student key = sys.students[i];
        int j = i - 1;
        while (j >= 0 && compareBuildingRoom(sys.students[j], key) > 0) {
            sys.students[j + 1] = sys.students[j];
            j--;
        }
        sys.students[j + 1] = key;
    }
}
