/* Day 14, part 2 = 100876 */

#include "reflector.h"

int main() {
    struct Reflector *reflector = getReflector();
    int answer = 0;

    if (reflector) {
        for (int i = 0; i < 1000; i++) {
            tilt(reflector, NORTH, true);
            tilt(reflector, WEST, false);
            tilt(reflector, SOUTH, true);
            tilt(reflector, EAST, false);
        }

        answer = totalLoad(reflector);

        freeReflector(reflector);
    }

    printf("%d\n", answer);
}
