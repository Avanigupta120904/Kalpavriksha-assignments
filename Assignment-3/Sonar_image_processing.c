#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generateMatrix(int size, int *matrix, int mxPixelVal) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < size * size; i++) {
        *(matrix + i) = rand() % (mxPixelVal + 1);
    }
}

void printMatrix(int size, int *matrix) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%3d ", *(matrix + row * size + col));
        }
        printf("\n");
    }
}

void rotateMatrixClockwise90(int size, int *matrix) {
    for (int layer = 0; layer < size/2; layer++){
        int firstIndex = layer;
        int lastIndex = size - 1 - layer;
        for (int offset = 0; offset < lastIndex - firstIndex; offset++) {
            int topPixel = *(matrix + firstIndex * size + firstIndex + offset);
        
            // left -> top
            *(matrix + firstIndex * size + firstIndex + offset) = *(matrix + (lastIndex - offset) * size + firstIndex);

            // bottom -> left
            *(matrix + (lastIndex - offset) * size + firstIndex) = *(matrix + lastIndex * size + lastIndex - offset);

            // right -> bottom
            *(matrix + lastIndex * size + lastIndex - offset) = *(matrix + (firstIndex + offset) * size + lastIndex);

            // top -> right
            *(matrix + (firstIndex + offset) * size + lastIndex) = topPixel;
        }
    }
}

void applyFilter(int size, int *matrix) {
    int *currentRowBuffer = (int *)malloc(size * sizeof(int));
    int *previousRowBuffer = (int *)malloc(size * sizeof(int));
    for (int col = 0; col < size; col++)
        previousRowBuffer[col] = *(matrix + 0 * size + col);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++)
            currentRowBuffer[col] = *(matrix + row * size + col);
        for (int col = 0; col < size; col++) {
            int sum = 0;
            int neighborCount = 0;
            for (int dr = -1; dr <= 1; dr++) {
                int neighborRow = row + dr;
                if (neighborRow < 0 || neighborRow >= size) continue;

                int *neighborRowPointer = (neighborRow == row - 1) ? previousRowBuffer :
                                          (neighborRow == row) ? currentRowBuffer :
                                                                 matrix + neighborRow * size;

                for (int dc = -1; dc <= 1; dc++) {
                    int neighborCol = col + dc;
                    if (neighborCol >= 0 && neighborCol < size) {
                        sum += neighborRowPointer[neighborCol];
                        neighborCount++;
                    }
                }
            }

            *(matrix + row * size + col) = sum / neighborCount;
        }
        for (int col = 0; col < size; col++)
            previousRowBuffer[col] = currentRowBuffer[col];
    }

    free(currentRowBuffer);
    free(previousRowBuffer);
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

    int *matrix = (int *)malloc(size * size * sizeof(int));
    if (!matrix) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    generateMatrix(size, matrix, mxPixelVal);
    printf("\nOriginal Randomly Generated Matrix:\n");
    printMatrix(size, matrix);

    rotateMatrixClockwise90(size, matrix);
    printf("\nMatrix after 90 degree Clockwise Rotation:\n");
    printMatrix(size, matrix);

    applyFilter(size, matrix);
    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    printMatrix(size, matrix);

    free(matrix);
    return 0;
}
