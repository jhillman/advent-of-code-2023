/* Day 1, part 1 = 54968 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char line[64];
        char *character;
        int digit1;
        int digit2;
        int answer = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            character = line;
            digit1 = -1;

            while (*character) {
                if (isdigit(*character)) {
                    if (digit1 == -1) {
                        digit1 = (*character - '0');
                    }

                    digit2 = (*character - '0');
                }

                character++;
            }

            answer += digit1 * 10 + digit2;
        }

        fclose(inputFile);

        printf("%d", answer);
    }

    return 0;
}
