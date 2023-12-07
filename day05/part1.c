/* Day 5, part 1 = 227653707 */

#include "garden.h"

int main() {
    struct Garden *garden = getGarden();

    if (garden) {
        struct Map *map;
        long answer = 1000000000000000000;

        for (int i = 0; i < garden->seeds->count; i++) {
            map = garden->map;
            long value = garden->seeds->data[i];

            while (map) {
                value = mapValue(value, map->ranges);

                map = map->next;
            }

            if (value < answer) {
                answer = value;
            }
        }

        freeGarden(garden);

        printf("%ld", answer);
    }

    return 0;
}
