# Dormitory Management System Design Spec

**Date**: 2026-06-04
**Course**: Data Structures Course Design (数据结构课程设计)
**Language**: C++
**Project root**: `/Users/craig/dormitory-system`

## Overview

A console-based dormitory management system that demonstrates data structure concepts including dynamic arrays, search algorithms (sequential, binary, block), and sorting algorithms (bubble, quick, insertion). No database — all data structures implemented from scratch.

## Data Model

```cpp
struct Student {
    char id[20];        // 学号 (unique key)
    char name[30];      // 姓名
    char gender[5];     // 性别
    char building[10];  // 宿舍楼号
    char room[10];      // 房间号
    char bed[5];        // 床位号
    char phone[15];     // 联系电话
    char major[30];     // 专业/院系
};

struct DormSystem {
    Student* students;  // Dynamic array
    int count;          // Current record count
    int capacity;       // Allocated capacity
};
```

- Dynamic array starts at capacity 100, doubles when full
- `id` is the unique primary key for all operations
- Persistence: text file `data.txt`, one record per line, fields delimited by `|`

## Project Structure

```
dormitory-system/
├── include/
│   ├── student.h      // Student struct, DormSystem struct, constants
│   ├── fileio.h       // File I/O function declarations
│   ├── search.h       // Search algorithm declarations
│   ├── sort.h         // Sort algorithm declarations
│   └── ui.h           // Menu and display function declarations
├── src/
│   ├── fileio.cpp     // Load/save data.txt
│   ├── search.cpp     // 3 search algorithms
│   ├── sort.cpp       // 3 sort algorithms
│   └── ui.cpp         // Menu system, input handling, display
├── main.cpp           // Entry point, initialization, main loop
├── data.txt           // Persistent data file (auto-created)
└── Makefile           // Build configuration
```

## Module 1: CRUD Operations

### Add (录入)
1. Prompt user for all 8 fields
2. Validate ID uniqueness (sequential scan)
3. If array at capacity, reallocate with 2x size
4. Append to end of array
5. Display success message

### Delete (删除)
1. Prompt for student ID
2. Sequential search to find record
3. Display record, confirm deletion
4. Shift all subsequent elements left by one
5. Decrement count

### Modify (修改)
1. Prompt for student ID
2. Sequential search to find record
3. Display current values
4. For each field: prompt for new value (Enter = keep current)
5. Update changed fields

### Display All (显示全部)
- Print all records in formatted table
- Show total count

## Module 2: Search Algorithms

### 1. Sequential Search (顺序查找)
- Search by any field (user chooses which field)
- Linear scan through array, O(n)
- Returns all matching records

### 2. Binary Search (折半查找)
- Search by student ID only
- Requires array to be sorted by ID first (auto-sort if needed)
- Standard binary search on sorted array, O(log n)
- Returns exact match

### 3. Block/Index Search (分块查找)
- Search by building number
- Build index table: each block stores (max_building, start_index) for a group of records sorted by building
- First find the block, then sequential search within block
- O(√n) average case

## Module 3: Sorting Algorithms

### 1. Bubble Sort (冒泡排序)
- Sort by student ID (ascending)
- Standard bubble sort with early termination flag
- O(n²), stable

### 2. Quick Sort (快速排序)
- Sort by student name (ascending, strcmp)
- Recursive quicksort with median-of-three pivot
- O(n log n) average, unstable

### 3. Insertion Sort (直接插入排序)
- Sort by building + room number
- Standard insertion sort on array
- O(n²), stable, good for small/nearly-sorted data

All sorts are in-place on the dynamic array. User selects sort key at runtime.

## UI Menu

```
========== 宿舍管理系统 ==========
1. 录入学生信息
2. 删除学生信息
3. 修改学生信息
4. 显示所有信息
5. 查找学生信息
6. 排序
7. 保存数据
8. 退出系统
==================================
请选择操作:
```

Sub-menus for search (5) and sort (6):
- Search: 1-顺序查找, 2-折半查找, 3-分块查找
- Sort: 1-冒泡排序(按学号), 2-快速排序(按姓名), 3-直接插入排序(按楼号房间)

Main loop: display menu → read choice → execute → loop back.

## File I/O

### data.txt format
```
2024001|张三|男|A|101|1|13800001111|计算机科学
2024002|李四|女|B|205|2|13800002222|软件工程
```

### Load (startup)
1. Open `data.txt` for reading
2. If file doesn't exist, initialize empty system
3. Parse each line, split by `|`, populate Student struct
4. Reallocate array if needed during load

### Save (manual + exit)
1. Open `data.txt` for writing (truncate)
2. Write each record as one line, fields joined by `|`
3. Flush and close

## Error Handling

- Invalid menu input: re-prompt
- ID not found: display message, return to menu
- File I/O errors: display warning, continue with in-memory data
- Array full: reallocate (handled transparently)
- Empty system operations: display "no records" message

## Test Data

Pre-populate data.txt with 10-15 sample records covering:
- Multiple buildings (A, B, C)
- Multiple rooms per building
- Both genders
- Different majors
- IDs in non-sorted order (to demonstrate sorting)

## Build

```makefile
CXX = g++
CXXFLAGS = -Wall -g
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
