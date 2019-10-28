#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h>

#define STRING_BUFFER 100
#define VALUE_SIZE 205000

void readFromFile(FILE* file, int* values) {
    for (int index = 0; fscanf(file, "%d", &*(values + index)) != EOF; index += 1){}
    fclose(file);
}

int main(int argc, char* argv[]) {
    printf("\nMultiple program launched!\n");

    // To check the number of arguments.
    if (argc == 3) {
        // To check the accessibility of files.
        FILE* fileOne = fopen(argv[1], "r");
        FILE* fileTwo = fopen(argv[2], "r");

        if (fileOne != NULL && fileTwo != NULL) {
            // Pipes for transferring data between parent and child process.
            int pipeOne[2];
            int pipeTwo[2];

            if (pipe(pipeOne) != -1 && pipe(pipeTwo) != -1) {
                pid_t process = fork();

                double mean = 0.0;
                double standard_deviation = 0.0;
                double sumParent = 0.0, sumChild = 0.0;
                double sigmaParent = 0.0, sigmaChild = 0.0;

                if (process > 0) {
                    /* ----- PARENT ----- */
                    // To assign data from the file to the array.             
                    printf("Reading %s\n", argv[1]);
                    int* fileOneValues = (int*)calloc(VALUE_SIZE, sizeof(int));
                    readFromFile(fileOne, fileOneValues);

                    /* ----- START OF TIMER ----- */
                    // To calculate elapsed time when finding standard deviation.
                    printf("Timer started.\n");
                    clock_t start = clock();

                    for (int index = 0; index < VALUE_SIZE; index += 1) {
                        sumParent += *(fileOneValues + index);
                    }
                    close(pipeOne[1]);  // Writing was closed to read.
                    close(pipeTwo[0]);  // Reading was closed to write.
                    read(pipeOne[0], &sumChild, sizeof(sumChild)); // To receive sum of child from child.

                    mean += (sumChild + sumParent) / (2 * VALUE_SIZE);
                    printf("The mean is %lf\n", mean);

                    write(pipeTwo[1], &mean, sizeof(mean)); // To send the mean to child.

                    for (int index = 0; index < VALUE_SIZE; index += 1) {
                        sigmaParent += pow(*(fileOneValues + index) - mean, 2);
                    }
                    read(pipeOne[0], &sigmaChild, sizeof(sigmaChild)); // To receive the sigma of the child from child.
                    printf("The standard deviation is %.5lf\n", sqrt((sigmaParent + sigmaChild) / (2 * VALUE_SIZE)));

                    /* ----- END OF TIMER ----- */                    
                    clock_t difference = clock() - start;
                    printf("Calculation completed in %ld milliseconds.\n",
                    (difference * 1000 / CLOCKS_PER_SEC % 1000));  
                } else if (process == 0) {    
                    /* ----- CHILD ----- */
                    // To assign data from the file to the array.                
                    printf("Reading %s\n", argv[2]);
                    int* fileTwoValues = (int*)calloc(VALUE_SIZE, sizeof(int));
                    readFromFile(fileTwo, fileTwoValues);

                    for (int index = 0; index < VALUE_SIZE; index += 1) {
                        sumChild += *(fileTwoValues + index);
                    }
                    close(pipeOne[0]);  // Reading was closed to write.
                    close(pipeTwo[1]);  // Writing was closed to read.

                    write(pipeOne[1], &sumChild, sizeof(sumChild)); // To send the sum of the child to parent.
                    read(pipeTwo[0], &mean, sizeof(mean)); // To receive the mean from parent.

                    for (int index = 0; index < VALUE_SIZE; index += 1) {
                        sigmaChild += pow(*(fileTwoValues + index) - mean, 2);
                    }
                    write(pipeOne[1], &sigmaChild, sizeof(sigmaChild)); // To send the sigma of the child to parent.
                } else {
                    printf("Pipes failed!");
                    exit(EXIT_FAILURE);
                }          
            } else {
                printf("Fork failed!");
                exit(EXIT_FAILURE);
            }
        } else {
            char error_message[STRING_BUFFER];
            sprintf(error_message, "\nCould not open file(s) %s%s%s ",
                fileOne == NULL ? argv[1] : "",
                fileOne == NULL && fileTwo == NULL ? " and " : "",
                fileTwo == NULL ? argv[2] : "");
            perror(error_message);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("You have to enter two arguments!");
    }
    return 0;
}
