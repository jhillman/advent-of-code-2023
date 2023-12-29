/* Day 23, part 2 = 6738 */

#include "map.h"

int main() {
    struct Map *map = getMap();
    int answer = 0;

    if (map) {
        answer = longestHike(map, DRY);

        freeMap(map);
    }

    printf("%d", answer);

    return 0;
}
