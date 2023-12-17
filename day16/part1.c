/* Day 16, part 1 = 7927 */

#include "energy.h"

int main() {
    struct Contraption *contraption = getContraption();
    int answer = 0;

    if (contraption) {
        struct Beams *beams = (struct Beams *)calloc(1, sizeof(struct Beams));

        addBeam(beams, (struct Beam){ 0, 0, RIGHT, true });

        answer = energizedTiles(contraption, beams);

        freeBeams(beams);
        freeContraption(contraption);
    }

    printf("%d", answer);

    return 0;
}
