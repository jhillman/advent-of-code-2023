/* Day 15, part 1 = 503154 */

#include <stdio.h>

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char c;
        int hash;
        int answer = 0;

        do {
            hash = 0;
            c = fgetc(inputFile);

            while (!feof(inputFile) && c != ',') {
                hash += c;
                hash *= 17;
                hash %= 256;

                c = fgetc(inputFile);
            }

            answer += hash;
        } while (!feof(inputFile));

        fclose(inputFile);

        printf("%d", answer);
    }

    return 0;
}
