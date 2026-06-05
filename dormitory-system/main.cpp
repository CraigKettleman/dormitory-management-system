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
