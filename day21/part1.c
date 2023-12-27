/* Day 21, part 1 = 3585 */

#include "farm.h"

int main() {
    struct Farm *farm = getFarm(64);
    long answer = 0;

    if (farm) {
        answer = reachablePlots(farm, farm->start, 64, 0);

        freeFarm(farm);
    }

    printf("%ld", answer);

    return 0;
}
