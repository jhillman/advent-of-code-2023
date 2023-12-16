/* Day 14, part 1 = 108935 */

#include "reflector.h"

int main() {
    struct Reflector *reflector = getReflector();
    int answer = 0;

    if (reflector) {
        tilt(reflector, NORTH, true);

        answer = totalLoad(reflector);

        freeReflector(reflector);
    }

    printf("%d\n", answer);
}
