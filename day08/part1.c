/* Day 8, part 1 = 16271 */

#include "map.h"

bool test(struct Node *node) {
    return strcmp(node->name, "ZZZ") == 0;
}

int main() {
    struct Map *map = getMap();

    if (map) {
        long answer = getSteps(map, map->nodes->data, test);

        freeMap(map);

        printf("%ld", answer);
    }

    return 0;
}
