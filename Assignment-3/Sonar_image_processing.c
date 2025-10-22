#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


void generateMatrix(int size, int matrix[][size], int mxPixelVal) {
    srand((unsigned int)time(NULL));

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            *(*(matrix + row) + col) = rand() % (mxPixelVal + 1);
        }
    }
}

void printMatrix(int size, int matrix[][size]) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%3d ", matrix[row][col]);
        }
        printf("\n");
    }
}

void rotateMatrixClockwise90(int size, int matrix[][size]) {
    for (int row = 0; row < size; row++) {
        for (int col = row; col < size; col++) {
            int temp = matrix[row][col];
           *(*(matrix + row) + col) = *(*(matrix + col) + row);
           *(*(matrix + col) + row) = temp;
        }
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size / 2; col++) {
            int temp = *(*(matrix + row) + col);
            *(*(matrix + row) + col) = *(*(matrix + row) + (size - 1 - col));
            *(*(matrix + row) + (size - 1 - col)) = temp;
        }
    }
}

void applyFilter(int size, int matrix[][size]) {
    int previousRowCopy[size];
    int currentRowCopy[size];

    memcpy(previousRowCopy, matrix[0], size * sizeof(int));

    for (int row = 0; row < size; row++) {
        memcpy(currentRowCopy, matrix[row], size * sizeof(int));

        for (int col = 0; col < size; col++) {
            int sum = 0;
            int count = 0;
            
            for (int y = -1; y <= 1; y++) {
                for (int x = -1; x <= 1; x++) {
                    int neighborRow = row + y;
                    int neighborCol = col + x;

                    if (neighborRow >= 0 && neighborRow < size && neighborCol >= 0 && neighborCol < size) {
                        if (neighborRow == row) {
                            sum += currentRowCopy[neighborCol];
                        } else if (neighborRow == row - 1) {
                            sum += previousRowCopy[neighborCol];
                        } else {
                            sum += matrix[neighborRow][neighborCol];
                        }
                        count++;
                    }
                }
            }

            matrix[row][col] = sum / count;
        }

        memcpy(previousRowCopy, currentRowCopy, size * sizeof(int));
    }
}

int main() {
    int size;
    const int mxPixelVal = 255; 

    printf("Enter the size of the matrix(2-10): ");

    while (1) {
        if (scanf("%d", &size) == 1 && size >= 2 && size <= 10) {
            // Clear buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            break;
        }
        printf("Invalid input. Please enter a number between 2 and 10: ");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    int matrix[size][size];

    generateMatrix(size, matrix, mxPixelVal);
    printf("\nOriginal Randomly Generated Matrix:\n");
    printMatrix(size, matrix);

    rotateMatrixClockwise90(size, matrix);
    printf("\nMatrix after 90 degree Clockwise Rotation:\n");
    printMatrix(size, matrix);

    applyFilter(size, matrix);
    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    printMatrix(size, matrix);

    return 0;
}
