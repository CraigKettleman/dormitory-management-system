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
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        if (sys.count >= sys.capacity) expandSystem(sys);

        Student& s = sys.students[sys.count];
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
