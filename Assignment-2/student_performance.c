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

int main() {
    int present[101];
    for(int i = 1; i <= 100; i++) present[i] = 0;
    //•	Constraints
    // 1.	Number of students: 1 ≤ N ≤ 100
    int n = 0;
    while(n < 1 || n > 100) {
        printf("Enter total number of students(1<=N<= 100): ");
        scanf("%d", &n);
        if(n < 1 || n > 100) printf("Invalid number!!\n");
    }

    struct Student students[101];

    for (int i = 0; i < n; i++) {
        printf("Enter Roll number for Student-%d: ", i+1);
        int rollNumber;
        scanf("%d",  &rollNumber);
        if(rollNumber <= 0) {
            printf("Enter a valid Roll Number(i.e Roll number must be greater than 0)\n");
            i--;
            continue;
        }
        else if(present[rollNumber] == 1) {
            printf("Roll Already Exists!\nEnter a unique Roll number!\n");
            i--;
            continue;
        }
        present[rollNumber] = 1;
        
        students[rollNumber].rollNo = rollNumber;
        printf("Enter Name for Roll number-%d: ", rollNumber);
        scanf("%s",  &students[rollNumber].name);


        //•	Constraints
        // 1.	Marks in each subject: 0 ≤ Marks ≤ 100
        for(int j = 0; j < 3; j++) {
            printf("Enter Marks%d for %s: ", j+1, students[rollNumber].name);
            scanf("%f", &students[rollNumber].marks[j]);
            if(students[rollNumber].marks[j] < 0 || students[rollNumber].marks[j] > 100) {
                printf("Marks must be between 0 and 100\nTry Again\n");
                j--;
            }
        }
        calculateGrades(&students[rollNumber]);
    }
    printDetails(students, present, 0);

    return 0;
}