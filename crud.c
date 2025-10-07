#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int id = 0;

struct User {
    int id;
    char name[50];
    int age;
};

void add() {
    struct User user;
    FILE *file = fopen("users.txt", "a");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("name: ");
    scanf("%s", user.name);
    printf("age: ");
    scanf("%d", &user.age);

    user.id = ++id;
    fprintf(file, "%d %s %d\n", user.id, user.name, user.age);
    printf("User Added:\nuserId: %d\nname: %s\nAge: %d\n", user.id, user.name, user.age);
    fclose(file);
}

void print() {
    struct User user;
    FILE *file = fopen("users.txt", "r");

    if (file == NULL) {
        printf("File Not Found.\n");
        return;
    }
    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) != EOF) {
        printf("ID: %d, Name: %s, Age: %d\n", user.id, user.name, user.age);
    }
    fclose(file);
}

void update() {
    struct User user;
    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL) {
        printf("File Not Found\n");
        return;
    }

    int temp_id, found = 0;
    printf("User id to update: ");
    scanf("%d", &temp_id);

    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) != EOF) {
        if (user.id == temp_id) {
            found = 1;
            printf("Enter new name: ");
            scanf("%s", user.name);
            printf("Enter new age: ");
            scanf("%d", &user.age);
        }
        fprintf(temp, "%d %s %d\n", user.id, user.name, user.age);
    }

    fclose(file);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("Updated!\n");
    else
        printf("NO user with %d id\n", temp_id);
}

void removeUser() {
    struct User user;
    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL) {
        printf("No file found\n");
        return;
    }

    int tempId, found = 0;
    printf("Enter User ID to delete: ");
    scanf("%d", &tempId);

    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) != EOF) {
        if (user.id != tempId)
            fprintf(temp, "%d %s %d\n", user.id, user.name, user.age);
        else
            found = 1;
    }

    fclose(file);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("Deleted!\n");
    else
        printf("NO user with %d id\n", tempId);
}

int main() {
    int choice;

    while (1) {
        printf("\n1. Add User\n");
        printf("2. Print Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add();
                break;
            case 2:
                print();
                break;
            case 3:
                update();
                break;
            case 4:
                removeUser();
                break;
            case 5:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Try Again\n");
        }
    }

    return 0;
}