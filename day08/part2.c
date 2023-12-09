/* Day 8, part 2 = 14265111103729 */

#include "map.h"

bool test(struct Node *node) {
    return node->name[2] == 'Z';
}

long gcd(long a, long b) {
    long temp;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

long lcm(long a, long b) {
    return a * b / gcd(a, b);
}

int main() {
    struct Map *map = getMap();

    if (map) {
        long *steps = NULL;
        int stepCount = 0;

        for (int i = 0; i < map->nodes->count; i++) {
            if (map->nodes->data[i].name[2] == 'A') {
                steps = (long *)realloc(steps, (stepCount + 1) * sizeof(long));
                steps[stepCount++] = getSteps(map, &map->nodes->data[i], test);
            }
        }

        long answer = *steps;

        for (int i = 1; i < stepCount; i++) {
            answer = lcm(answer, steps[i]);
        }

        free(steps);
        freeMap(map);

        printf("%ld", answer);
    }

    return 0;
}
