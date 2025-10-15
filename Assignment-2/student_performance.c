#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Student {
    int rollNo;
    char name[100];
    float marks[3];
    float total;
    float average;
    char grade;
};

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

void printDetails(struct Student *students, int *present, int currStudent) {
    if(currStudent > 100) return;
    if(present[currStudent] == 1) {

        printf("\nRoll: %d\n", students[currStudent].rollNo);
        printf("Name: %s\n", students[currStudent].name);
        printf("Total: %.2f\n", students[currStudent].total);
        printf("Average: %.2f\n", students[currStudent].average);
        printf("Grade: %c\n", students[currStudent].grade);

        switch (students[currStudent].grade) {
            case 'A': printf("*****\n"); break;
            case 'B': printf("****\n"); break;
            case 'C': printf("***\n"); break;
            case 'D': printf("**\n"); break;
        }
    }
    printDetails(students, present, currStudent+1);
}

void initializePresentArray(int present[]) {
    for (int i = 1; i <= 100; i++)
        present[i] = 0;
}

int getNumberOfStudents() {
    int n = 0;
    char ch;
    while (1) {
        printf("Enter total number of students (1 <= N <= 100): ");
        if (scanf("%d", &n) != 1) {  
            while ((ch = getchar()) != '\n' && ch != EOF); 
            printf("Invalid number!! Please try again.\n");
            continue;
        }
        if (n < 1 || n > 100){
            printf("Invalid number!! Please try again.\n");
            continue;
    }
    break;
}
    return n;
} 

void inputName(char name[]) {
    int valid;
    char temp[100];

    while (1) {
        scanf("%s", temp);  // read input
        valid = 1;
        for (int i = 0; temp[i] != '\0'; i++) {
            if (!isalpha(temp[i])) {  // check if each character is a letter
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("Name should contain only letters. Try again: ");
        } else {
            strcpy(name, temp);
            break;
        }
    }
}


int getValidRollNumber(int present[]) {
    int rollNumber;
    char ch;
    while (1) {
        if (scanf("%d", &rollNumber) != 1) {  // non-numeric input
            while ((ch = getchar()) != '\n' && ch != EOF); // clear buffer
            printf("Enter a valid Roll Number(i.e Roll number must be greater than 0)\n");
            continue;
        }
        if (rollNumber <= 0) {
            printf("Enter a valid Roll Number(i.e Roll number must be greater than 0)\n");
        } else if (present[rollNumber] == 1) {
            printf("Roll Already Exists!\nEnter a unique Roll number!\n");
        } else {
            present[rollNumber] = 1;
            break;
        }
        printf("Enter Roll number: "); // 
    }
    return rollNumber;
}

void inputMarks(struct Student *student) {
    float mark;
    char ch;  // to clear invalid input

    for (int j = 0; j < 3; j++) {
        while (1) {
            printf("Enter Marks%d for %s: ", j + 1, student->name);
            
            if (scanf("%f", &mark) != 1) {  // non-numeric input
                while ((ch = getchar()) != '\n' && ch != EOF); // clear buffer
                printf("Marks must be between 0 and 100\nTry Again\n");
                continue;
            }

            if (mark < 0 || mark > 100) {
                printf("Marks must be between 0 and 100\nTry Again\n");
            } else {
                student->marks[j] = mark;
                break;
            }
        }
    }
}

void inputStudentData(struct Student students[], int present[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Enter Roll number for Student-%d: ", i + 1);
        int rollNumber = getValidRollNumber(present);

        students[rollNumber].rollNo = rollNumber;
        printf("Enter Name for Roll number-%d: ", rollNumber);
        inputName(students[rollNumber].name);

        inputMarks(&students[rollNumber]);
        calculateGrades(&students[rollNumber]);
    }
}

void processAll(struct Student students[], int present[], int n) {
    printDetails(students, present, 0);
}

int main() {
    int present[101];
    struct Student students[101];

    initializePresentArray(present);
    int n = getNumberOfStudents();
    inputStudentData(students, present, n);
    processAll(students, present, n);

    return 0;
}