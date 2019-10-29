#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define STRING_BUFFER 100
#define VALUE_SIZE 205000

void readFromFile(FILE * file, int * values) {
    int index = 0;
    for (index = 0; fscanf(file, "%d", & * (values + index)) != EOF; index += 1) {}
    fclose(file);
}

void findStandardDeviation(int * fileOneValues, int * fileTwoValues) {
    int index = 0;
    double sum = 0.0;
    double mean = 0.0;
    double standardDeviation = 0.0;

    for (index = 0; index < VALUE_SIZE; index += 1) {
        sum += * (fileOneValues + index);
        sum += * (fileTwoValues + index);
    }
    mean = sum / (2 * VALUE_SIZE);
    printf("The mean is: %.5lf\n", mean);

    for (index = 0; index < VALUE_SIZE; index += 1) {
        standardDeviation += pow( * (fileOneValues + index) - mean, 2);
        standardDeviation += pow( * (fileTwoValues + index) - mean, 2);
    }
    printf("The standard deviation is %.5lf\n", sqrt(standardDeviation / (2 * VALUE_SIZE)));
}

int main(int argc, char * argv[]) {
    // To check the number of arguments.
    if (argc == 3) {
        // To check the accessibility of files.
        FILE * fileOne = fopen(argv[1], "r");
        FILE * fileTwo = fopen(argv[2], "r");

        if (fileOne != NULL && fileTwo != NULL) {
            // To store data read from the file.
            printf("Sequential program launched!\n");
            int * fileOneValues = (int * ) calloc(VALUE_SIZE, sizeof(int));
            int * fileTwoValues = (int * ) calloc(VALUE_SIZE, sizeof(int));

            printf("Reading %s\n", argv[1]);
            readFromFile(fileOne, fileOneValues);
            printf("Reading %s\n", argv[2]);
            readFromFile(fileTwo, fileTwoValues);

            // To calculate the elapsed time during operation.
            printf("Timer started\n");
            clock_t start = clock();
            findStandardDeviation(fileOneValues, fileTwoValues);
            clock_t difference = clock() - start;
            printf("Calculation completed in %ld milliseconds\n",
                (difference * 1000 / CLOCKS_PER_SEC % 1000));
        } else {
            char error_message[STRING_BUFFER];
            sprintf(error_message, "Could not open file(s) %s%s%s\n",
                fileOne == NULL ? argv[1] : "",
                fileOne == NULL && fileTwo == NULL ? " and " : "",
                fileTwo == NULL ? argv[2] : "");
            perror(error_message);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("You have to enter two arguments!\n");
    }
    return 0;
}
