#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

enum ExtrapolationType {
    NEXT,
    PREVIOUS
};

struct Values {
    int count;
    long *data;
};

long extrapolatedValue(struct Values values, enum ExtrapolationType type) {
    struct Values differences = { values.count - 1, malloc((values.count - 1) * sizeof(long)) };
    bool allZero = true;

    for (int i = 0; i < values.count - 1; i++) {
        differences.data[i] = values.data[i + 1] - values.data[i];
        allZero = allZero && differences.data[i] == 0;
    }

    if (allZero) {
        free(differences.data);

        return type == NEXT ? values.data[values.count - 1] : values.data[0];
    } else {
        long extrapolated = extrapolatedValue(differences, type);

        free(differences.data);

        return type == NEXT ? values.data[values.count - 1] + extrapolated : values.data[0] - extrapolated;
    }
}

long getExtrapolatedSum(enum ExtrapolationType type) {
    FILE *inputFile = fopen("input.txt", "r");
    long sum = 0;

    if (inputFile) {
        char c;
        struct Values values = { 0, NULL };

        do {
            c = fgetc(inputFile);
    
            ++values.count;

            while (isdigit(c)) {
                c = fgetc(inputFile);
            } 
        } while (c != '\n');

        values.data = (long *)malloc(values.count * sizeof(long));

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile)) {
            for (int i = 0; i < values.count; i++) {
                fscanf(inputFile, "%ld", &values.data[i]);
            }

            sum += extrapolatedValue(values, type);
        }

        fclose(inputFile);

        free(values.data);
    }

    return sum;
}