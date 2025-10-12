#include <stdio.h>
#include <string.h>

struct Student {
    int rollNo;
    char name[100];
    float marks[3];
    float total;
    float average;
    char grade;
};

int totalStudents;


void calculateGrades(struct Student *student) {
    student->total = 0;
    for (int i = 0; i < 3; i++)
        student->total += student->marks[i];
    
    student->average = student->total / 3;

    if (student->average >= 85)
        student->grade = 'A';
    else if (student->average >= 70)
        student->grade = 'B';
    else if (student->average >= 50)
        student->grade = 'C';
    else if (student->average >= 35)
        student->grade = 'D';
    else
        student->grade = 'F';
}

void printDetails(struct Student *students, int totalStudents) {
    for (int i = 0; i < totalStudents; i++) {
        printf("\nStudent %d Details:\n", i + 1);
        printf("Roll: %d\n", students[i].rollNo);
        printf("Name: %s\n", students[i].name);
        printf("Total: %.2f\n", students[i].total);
        printf("Average: %.2f\n", students[i].average);
        printf("Grade: %c\n", students[i].grade);

        switch (students[i].grade) {
            case 'A': printf("*****\n"); break;
            case 'B': printf("****\n"); break;
            case 'C': printf("***\n"); break;
            case 'D': printf("**\n"); break;
            default: continue;
        }
    }
}

int main() {
    scanf("%d", &totalStudents);

    struct Student students[totalStudents];

    for (int i = 0; i < totalStudents; i++) {
        scanf("%d %s %f %f %f", &students[i].rollNo, &students[i].name, &students[i].marks[0], &students[i].marks[1], &students[i].marks[2]);
        calculateGrades(&students[i]);
    }

    printDetails(students, totalStudents);

    return 0;
}