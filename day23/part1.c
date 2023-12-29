/* Day 23, part 1 = 2310 */

#include "map.h"

int main() {
    struct Map *map = getMap();
    int answer = 0;

    if (map) {
        answer = longestHike(map, ICY);

        freeMap(map);
    }

    printf("%d", answer);

    return 0;
}
