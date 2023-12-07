/* Day 5, part 2 = 78775051 */

#include "garden.h"

int main() {
    struct Garden *garden = getGarden();

    if (garden) {
        struct Map *map;
        long answer = 1000000000000000000;

        for (int i = 0; i < garden->seeds->count; i += 2) {
            for (int j = 0; j < garden->seeds->data[i + 1]; j++) {
                map = garden->map;
                long value = garden->seeds->data[i] + j;

                while (map) {
                    value = mapValue(value, map->ranges);

                    map = map->next;
                }

                if (value < answer) {
                    answer = value;
                }
            }
        }

        freeGarden(garden);

        printf("%ld", answer);
    }

    return 0;
}
