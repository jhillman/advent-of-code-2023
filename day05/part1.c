/* Day 5, part 1 = 227653707 */

#include "garden.h"

int main() {
    struct Garden *garden = getGarden(false);

    if (garden) {
        long answer = 0;

        for (int i = 0; i < garden->seeds->count; i++) {
            long value = processValue(garden, garden->seeds->data[i]);

            if (!answer || value < answer) {
                answer = value;
            }
        }

        freeGarden(garden);

        printf("%ld", answer);
    }

    return 0;
}
