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
