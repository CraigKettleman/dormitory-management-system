# Dormitory Management System Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build a console-based dormitory management system in C++ with CRUD, 3 search algorithms, and 3 sorting algorithms.

**Architecture:** Dynamic array of Student structs as the primary data structure. Modular design with separate header/source files for data model, file I/O, search, sort, and UI. Text file persistence.

**Tech Stack:** C++, g++, Makefile

---

## File Map

| File | Responsibility |
|------|---------------|
| `include/student.h` | Student/DormSystem structs, constants |
| `include/fileio.h` | File I/O declarations |
| `include/search.h` | Search algorithm declarations |
| `include/sort.h` | Sort algorithm declarations |
| `include/ui.h` | Menu/display declarations |
| `src/fileio.cpp` | Load/save data.txt |
| `src/search.cpp` | Sequential, binary, block search |
| `src/sort.cpp` | Bubble, quick, insertion sort |
| `src/ui.cpp` | CRUD operations, menu system, display |
| `main.cpp` | Entry point, main loop |
| `Makefile` | Build configuration |
| `data.txt` | Persistent data (sample records) |

---

### Task 1: Project Scaffolding

**Files:**
- Create: `Makefile`
- Create: `include/` directory
- Create: `src/` directory

- [ ] **Step 1: Create directory structure**

```bash
cd /Users/craig/dormitory-system
mkdir -p include src
```

- [ ] **Step 2: Create Makefile**

```makefile
CXX = g++
CXXFLAGS = -Wall -g -std=c++11
INCLUDES = -Iinclude
TARGET = dormitory

SRCS = main.cpp src/fileio.cpp src/search.cpp src/sort.cpp src/ui.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
```

- [ ] **Step 3: Verify Makefile syntax**

Run: `cd /Users/craig/dormitory-system && make clean`
Expected: `rm -f ...` (no errors, just nothing to clean)

- [ ] **Step 4: Commit**

```bash
git add Makefile include/ src/
git commit -m "chore: create project structure with Makefile"
```

---

### Task 2: Data Model Header

**Files:**
- Create: `include/student.h`

- [ ] **Step 1: Write student.h**

```cpp
#ifndef STUDENT_H
#define STUDENT_H

#define MAX_ID_LEN      20
#define MAX_NAME_LEN    30
#define MAX_GENDER_LEN  5
#define MAX_BUILDING_LEN 10
#define MAX_ROOM_LEN    10
#define MAX_BED_LEN     5
#define MAX_PHONE_LEN   15
#define MAX_MAJOR_LEN   30
#define INITIAL_CAPACITY 100

struct Student {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char gender[MAX_GENDER_LEN];
    char building[MAX_BUILDING_LEN];
    char room[MAX_ROOM_LEN];
    char bed[MAX_BED_LEN];
    char phone[MAX_PHONE_LEN];
    char major[MAX_MAJOR_LEN];
};

struct DormSystem {
    Student* students;
    int count;
    int capacity;
};

#endif // STUDENT_H
```

- [ ] **Step 2: Commit**

```bash
git add include/student.h
git commit -m "feat: add Student and DormSystem data structures"
```

---

### Task 3: File I/O Header and Implementation

**Files:**
- Create: `include/fileio.h`
- Create: `src/fileio.cpp`

- [ ] **Step 1: Write fileio.h**

```cpp
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
```

- [ ] **Step 2: Write src/fileio.cpp**

```cpp
#include "fileio.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

void initSystem(DormSystem& sys) {
    sys.capacity = INITIAL_CAPACITY;
    sys.count = 0;
    sys.students = (Student*)malloc(sizeof(Student) * sys.capacity);
}

static void expandSystem(DormSystem& sys) {
    sys.capacity *= 2;
    sys.students = (Student*)realloc(sys.students, sizeof(Student) * sys.capacity);
}

int loadFromFile(DormSystem& sys, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        if (sys.count >= sys.capacity) expandSystem(sys);

        Student& s = sys.students[sys.count];
        // Parse pipe-delimited fields
        char* token = strtok(line, "|");
        if (!token) continue;
        strncpy(s.id, token, MAX_ID_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(s.name, token, MAX_NAME_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(s.gender, token, MAX_GENDER_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(s.building, token, MAX_BUILDING_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(s.room, token, MAX_ROOM_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(s.bed, token, MAX_BED_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(s.phone, token, MAX_PHONE_LEN - 1);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(s.major, token, MAX_MAJOR_LEN - 1);

        sys.count++;
    }

    fclose(fp);
    return sys.count;
}

int saveToFile(DormSystem& sys, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("错误: 无法打开文件 %s 进行写入\n", filename);
        return -1;
    }

    for (int i = 0; i < sys.count; i++) {
        Student& s = sys.students[i];
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%s|%s\n",
                s.id, s.name, s.gender, s.building,
                s.room, s.bed, s.phone, s.major);
    }

    fclose(fp);
    return sys.count;
}

void destroySystem(DormSystem& sys) {
    free(sys.students);
    sys.students = NULL;
    sys.count = 0;
    sys.capacity = 0;
}
```

- [ ] **Step 3: Commit**

```bash
git add include/fileio.h src/fileio.cpp
git commit -m "feat: implement file I/O for loading and saving student data"
```

---

### Task 4: Search Header and Implementation

**Files:**
- Create: `include/search.h`
- Create: `src/search.cpp`

- [ ] **Step 1: Write search.h**

```cpp
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
```

- [ ] **Step 2: Write src/search.cpp**

```cpp
#include "search.h"
#include <cstring>

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
        // Use last element's building as block max
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
```

- [ ] **Step 3: Add `<cmath>` include for sqrt**

```cpp
#include <cmath>  // Add at top of src/search.cpp
```

- [ ] **Step 4: Commit**

```bash
git add include/search.h src/search.cpp
git commit -m "feat: implement sequential, binary, and block search algorithms"
```

---

### Task 5: Sort Header and Implementation

**Files:**
- Create: `include/sort.h`
- Create: `src/sort.cpp`

- [ ] **Step 1: Write sort.h**

```cpp
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
```

- [ ] **Step 2: Write src/sort.cpp**

```cpp
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
```

- [ ] **Step 3: Commit**

```bash
git add include/sort.h src/sort.cpp
git commit -m "feat: implement bubble sort, quick sort, and insertion sort"
```

---

### Task 6: UI Header and Implementation

**Files:**
- Create: `include/ui.h`
- Create: `src/ui.cpp`

- [ ] **Step 1: Write ui.h**

```cpp
#ifndef UI_H
#define UI_H

#include "student.h"

// Display main menu and return user choice (1-8)
int showMainMenu();

// CRUD operations
void addStudent(DormSystem& sys);
void deleteStudent(DormSystem& sys);
void modifyStudent(DormSystem& sys);
void displayAll(DormSystem& sys);

// Search sub-menu
void searchMenu(DormSystem& sys);

// Sort sub-menu
void sortMenu(DormSystem& sys);

// Display a single student record
void printStudent(const Student& s);

// Display table header
void printTableHeader();

#endif // UI_H
```

- [ ] **Step 2: Write src/ui.cpp**

```cpp
#include "ui.h"
#include "search.h"
#include "sort.h"
#include <cstdio>
#include <cstring>

static FILE* fp = stdin; // For potential redirection

void printTableHeader() {
    printf("%-12s %-10s %-4s %-6s %-6s %-4s %-13s %-15s\n",
           "学号", "姓名", "性别", "楼号", "房间", "床号", "联系电话", "专业");
    printf("------------------------------------------------------------------------\n");
}

void printStudent(const Student& s) {
    printf("%-12s %-10s %-4s %-6s %-6s %-4s %-13s %-15s\n",
           s.id, s.name, s.gender, s.building, s.room, s.bed, s.phone, s.major);
}

int showMainMenu() {
    printf("\n");
    printf("========== 宿舍管理系统 ==========\n");
    printf("1. 录入学生信息\n");
    printf("2. 删除学生信息\n");
    printf("3. 修改学生信息\n");
    printf("4. 显示所有信息\n");
    printf("5. 查找学生信息\n");
    printf("6. 排序\n");
    printf("7. 保存数据\n");
    printf("8. 退出系统\n");
    printf("==================================\n");
    printf("请选择操作: ");

    int choice;
    scanf("%d", &choice);
    while (getchar() != '\n'); // Clear input buffer
    return choice;
}

// Check if ID already exists
static int findById(DormSystem& sys, const char* id) {
    for (int i = 0; i < sys.count; i++) {
        if (strcmp(sys.students[i].id, id) == 0)
            return i;
    }
    return -1;
}

// Expand array if needed
static void expandIfNeeded(DormSystem& sys) {
    if (sys.count >= sys.capacity) {
        sys.capacity *= 2;
        sys.students = (Student*)realloc(sys.students, sizeof(Student) * sys.capacity);
        printf("(系统已自动扩展存储容量)\n");
    }
}

void addStudent(DormSystem& sys) {
    printf("\n--- 录入学生信息 ---\n");

    expandIfNeeded(sys);
    Student& s = sys.students[sys.count];

    printf("请输入学号: ");
    scanf("%19s", s.id);
    while (getchar() != '\n');

    if (findById(sys, s.id) >= 0 && findById(sys, s.id) < sys.count) {
        printf("错误: 学号 %s 已存在!\n", s.id);
        return;
    }

    printf("请输入姓名: ");
    scanf("%29s", s.name);
    while (getchar() != '\n');

    printf("请输入性别(男/女): ");
    scanf("%4s", s.gender);
    while (getchar() != '\n');

    printf("请输入宿舍楼号: ");
    scanf("%9s", s.building);
    while (getchar() != '\n');

    printf("请输入房间号: ");
    scanf("%9s", s.room);
    while (getchar() != '\n');

    printf("请输入床位号: ");
    scanf("%4s", s.bed);
    while (getchar() != '\n');

    printf("请输入联系电话: ");
    scanf("%14s", s.phone);
    while (getchar() != '\n');

    printf("请输入专业/院系: ");
    scanf("%29s", s.major);
    while (getchar() != '\n');

    sys.count++;
    printf("录入成功! 当前共有 %d 条记录.\n", sys.count);
}

void deleteStudent(DormSystem& sys) {
    printf("\n--- 删除学生信息 ---\n");
    if (sys.count == 0) {
        printf("当前没有学生记录.\n");
        return;
    }

    char id[MAX_ID_LEN];
    printf("请输入要删除的学生学号: ");
    scanf("%19s", id);
    while (getchar() != '\n');

    int idx = findById(sys, id);
    if (idx < 0) {
        printf("未找到学号为 %s 的学生.\n", id);
        return;
    }

    printf("找到以下学生:\n");
    printTableHeader();
    printStudent(sys.students[idx]);

    printf("确认删除? (y/n): ");
    char confirm;
    scanf("%c", &confirm);
    while (getchar() != '\n');

    if (confirm == 'y' || confirm == 'Y') {
        for (int i = idx; i < sys.count - 1; i++) {
            sys.students[i] = sys.students[i + 1];
        }
        sys.count--;
        printf("删除成功! 当前共有 %d 条记录.\n", sys.count);
    } else {
        printf("已取消删除.\n");
    }
}

void modifyStudent(DormSystem& sys) {
    printf("\n--- 修改学生信息 ---\n");
    if (sys.count == 0) {
        printf("当前没有学生记录.\n");
        return;
    }

    char id[MAX_ID_LEN];
    printf("请输入要修改的学生学号: ");
    scanf("%19s", id);
    while (getchar() != '\n');

    int idx = findById(sys, id);
    if (idx < 0) {
        printf("未找到学号为 %s 的学生.\n", id);
        return;
    }

    Student& s = sys.students[idx];
    printf("当前信息:\n");
    printTableHeader();
    printStudent(s);
    printf("\n(直接按回车跳过不修改的字段)\n");

    char buf[64];

    printf("姓名 [%s]: ", s.name);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(s.name, buf, MAX_NAME_LEN - 1);

    printf("性别 [%s]: ", s.gender);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(s.gender, buf, MAX_GENDER_LEN - 1);

    printf("楼号 [%s]: ", s.building);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(s.building, buf, MAX_BUILDING_LEN - 1);

    printf("房间 [%s]: ", s.room);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(s.room, buf, MAX_ROOM_LEN - 1);

    printf("床号 [%s]: ", s.bed);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(s.bed, buf, MAX_BED_LEN - 1);

    printf("电话 [%s]: ", s.phone);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(s.phone, buf, MAX_PHONE_LEN - 1);

    printf("专业 [%s]: ", s.major);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(s.major, buf, MAX_MAJOR_LEN - 1);

    printf("修改成功!\n");
}

void displayAll(DormSystem& sys) {
    printf("\n--- 所有学生信息 ---\n");
    if (sys.count == 0) {
        printf("当前没有学生记录.\n");
        return;
    }
    printTableHeader();
    for (int i = 0; i < sys.count; i++) {
        printStudent(sys.students[i]);
    }
    printf("------------------------------------------------------------------------\n");
    printf("共 %d 条记录\n", sys.count);
}

static void sequentialSearchMenu(DormSystem& sys) {
    printf("\n--- 顺序查找 ---\n");
    printf("请选择查找字段:\n");
    printf("1.学号  2.姓名  3.性别  4.楼号  5.房间  6.电话  7.专业\n");
    printf("请选择: ");

    int field;
    scanf("%d", &field);
    while (getchar() != '\n');

    if (field < 1 || field > 7) {
        printf("无效选择.\n");
        return;
    }

    char keyword[50];
    printf("请输入查找关键字: ");
    scanf("%49s", keyword);
    while (getchar() != '\n');

    int* indices = new int[sys.count];
    int matchCount = sequentialSearch(sys, field - 1, keyword, indices);

    if (matchCount == 0) {
        printf("未找到匹配的记录.\n");
    } else {
        printf("找到 %d 条匹配记录:\n", matchCount);
        printTableHeader();
        for (int i = 0; i < matchCount; i++) {
            printStudent(sys.students[indices[i]]);
        }
    }
    delete[] indices;
}

static void binarySearchMenu(DormSystem& sys) {
    printf("\n--- 折半查找 ---\n");
    printf("注意: 折半查找需要先按学号排序.\n");

    char id[MAX_ID_LEN];
    printf("请输入要查找的学号: ");
    scanf("%19s", id);
    while (getchar() != '\n');

    ensureSortedById(sys);
    int idx = binarySearchById(sys, id);

    if (idx < 0) {
        printf("未找到学号为 %s 的学生.\n", id);
    } else {
        printf("找到以下学生:\n");
        printTableHeader();
        printStudent(sys.students[idx]);
    }
}

static void blockSearchMenu(DormSystem& sys) {
    printf("\n--- 分块查找 ---\n");

    char building[MAX_BUILDING_LEN];
    printf("请输入要查找的楼号: ");
    scanf("%9s", building);
    while (getchar() != '\n');

    int idx = blockSearchByBuilding(sys, building);

    if (idx < 0) {
        printf("未找到楼号为 %s 的学生.\n", building);
    } else {
        printf("找到楼号 %s 的学生:\n", building);
        printTableHeader();
        // Show all students in that building
        int* indices = new int[sys.count];
        int count = sequentialSearch(sys, SEARCH_BY_BUILDING, building, indices);
        for (int i = 0; i < count; i++) {
            printStudent(sys.students[indices[i]]);
        }
        printf("共 %d 条记录\n", count);
        delete[] indices;
    }
}

void searchMenu(DormSystem& sys) {
    if (sys.count == 0) {
        printf("当前没有学生记录.\n");
        return;
    }

    printf("\n--- 查找学生信息 ---\n");
    printf("1. 顺序查找\n");
    printf("2. 折半查找\n");
    printf("3. 分块查找\n");
    printf("请选择查找方式: ");

    int choice;
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {
        case 1: sequentialSearchMenu(sys); break;
        case 2: binarySearchMenu(sys); break;
        case 3: blockSearchMenu(sys); break;
        default: printf("无效选择.\n");
    }
}

void sortMenu(DormSystem& sys) {
    if (sys.count == 0) {
        printf("当前没有学生记录.\n");
        return;
    }

    printf("\n--- 排序 ---\n");
    printf("1. 冒泡排序（按学号）\n");
    printf("2. 快速排序（按姓名）\n");
    printf("3. 直接插入排序（按楼号+房间）\n");
    printf("请选择排序方式: ");

    int choice;
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {
        case 1:
            bubbleSortById(sys);
            printf("已按学号排序(冒泡排序).\n");
            break;
        case 2:
            quickSortByName(sys);
            printf("已按姓名排序(快速排序).\n");
            break;
        case 3:
            insertionSortByBuildingRoom(sys);
            printf("已按楼号+房间排序(直接插入排序).\n");
            break;
        default:
            printf("无效选择.\n");
            return;
    }

    printf("\n排序结果:\n");
    displayAll(sys);
}
```

- [ ] **Step 3: Commit**

```bash
git add include/ui.h src/ui.cpp
git commit -m "feat: implement CRUD operations, search menu, and sort menu"
```

---

### Task 7: Main Entry Point

**Files:**
- Create: `main.cpp`

- [ ] **Step 1: Write main.cpp**

```cpp
#include <cstdio>
#include "student.h"
#include "fileio.h"
#include "ui.h"

int main() {
    DormSystem sys;
    initSystem(sys);

    int loaded = loadFromFile(sys, "data.txt");
    if (loaded > 0) {
        printf("已加载 %d 条学生记录.\n", loaded);
    } else {
        printf("未找到数据文件，将从空系统开始.\n");
    }

    bool running = true;
    while (running) {
        int choice = showMainMenu();
        switch (choice) {
            case 1: addStudent(sys); break;
            case 2: deleteStudent(sys); break;
            case 3: modifyStudent(sys); break;
            case 4: displayAll(sys); break;
            case 5: searchMenu(sys); break;
            case 6: sortMenu(sys); break;
            case 7:
                if (saveToFile(sys, "data.txt") >= 0) {
                    printf("数据已保存.\n");
                }
                break;
            case 8:
                saveToFile(sys, "data.txt");
                printf("数据已保存，退出系统.\n");
                running = false;
                break;
            default:
                printf("无效选择，请重新输入.\n");
        }
    }

    destroySystem(sys);
    return 0;
}
```

- [ ] **Step 2: Commit**

```bash
git add main.cpp
git commit -m "feat: add main entry point with menu loop"
```

---

### Task 8: Test Data

**Files:**
- Create: `data.txt`

- [ ] **Step 1: Create sample data.txt**

```
2024001|张三|男|A|101|1|13800001111|计算机科学
2024005|王芳|女|B|205|2|13800005555|软件工程
2024003|李四|男|A|102|3|13800003333|信息安全
2024002|赵丽|女|C|301|1|13800002222|人工智能
2024008|刘伟|男|A|101|2|13800008888|计算机科学
2024004|陈静|女|B|205|1|13800004444|软件工程
2024006|杨洋|男|C|302|2|13800006666|数据科学
2024009|黄蕾|女|A|103|1|13800009999|人工智能
2024007|周杰|男|B|206|3|13800007777|信息安全
2024010|吴敏|女|C|301|2|13800010000|计算机科学
2024011|孙强|男|A|102|1|13800011111|软件工程
2024012|郑雪|女|B|205|3|13800012222|数据科学
```

- [ ] **Step 2: Commit**

```bash
git add data.txt
git commit -m "feat: add sample test data with 12 student records"
```

---

### Task 9: Build and Verify

**Files:**
- Modify: `src/search.cpp` (add missing `#include <cmath>`)

- [ ] **Step 1: Build the project**

```bash
cd /Users/craig/dormitory-system && make clean && make
```

Expected: Successful compilation with no errors.

- [ ] **Step 2: Fix any compilation errors**

If `sqrt` is undefined, add `#include <cmath>` to `src/search.cpp`.
If there are other errors, fix them before proceeding.

- [ ] **Step 3: Run the program and test basic operations**

```bash
echo "4\n8\n" | ./dormitory
```

Expected: Displays all 12 records, then exits.

- [ ] **Step 4: Commit any fixes**

```bash
git add -A
git commit -m "fix: resolve compilation issues"
```

---

### Task 10: Final Integration Test

- [ ] **Step 1: Test all menu options manually**

Run: `./dormitory` and test each option:
1. Add a new student → verify success message
2. Delete a student → verify removal
3. Modify a student → verify changes
4. Display all → verify table format
5. Search (all 3 types) → verify results
6. Sort (all 3 types) → verify ordering
7. Save → verify data.txt updated
8. Exit → verify clean shutdown

- [ ] **Step 2: Final commit**

```bash
git add -A
git commit -m "feat: complete dormitory management system"
```
