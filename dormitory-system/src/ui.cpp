#include "ui.h"
#include "search.h"
#include "sort.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

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
