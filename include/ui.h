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
