/* Day 24, part 2 = 677656046662770 */

#include "hailstones.h"

int main() {
    struct Hailstones *hailstones = getHailstones();
    long answer = 0;

    if (hailstones) {
        long long rockVelocityX = findRockVelocity(hailstones, X);
        long long rockVelocityY = findRockVelocity(hailstones, Y);
        long long rockVelocityZ = findRockVelocity(hailstones, Z);

        // y = mx + b
        struct Hailstone hailstoneOne = hailstones->data[0];
        struct Hailstone hailstoneTwo = hailstones->data[1];

        double hailstoneOneM = (double)(hailstoneOne.velocity.y - rockVelocityY) / (double)(hailstoneOne.velocity.x - rockVelocityX);
        double hailstoneTwoM = (double)(hailstoneTwo.velocity.y - rockVelocityY) / (double)(hailstoneTwo.velocity.x - rockVelocityX);
        double hailstoneOneB = hailstoneOne.position.y - (hailstoneOneM * hailstoneOne.position.x);
        double hailstoneTwoB = hailstoneTwo.position.y - (hailstoneTwoM * hailstoneTwo.position.x);

        long long rockPositionX = (long long)((hailstoneTwoB - hailstoneOneB) / (hailstoneOneM - hailstoneTwoM));
        long long rockPositionY = (long long)(hailstoneOneM * rockPositionX + hailstoneOneB);
        long long rockTime = (rockPositionX - hailstoneOne.position.x) / (hailstoneOne.velocity.x - rockVelocityX);
        long long rockPositionZ = hailstoneOne.position.z + (hailstoneOne.velocity.z - rockVelocityZ) * rockTime;

        answer = rockPositionX + rockPositionY + rockPositionZ;

        free(hailstones->data);
        free(hailstones);
    }

    printf("%ld", answer);

    return 0;
}
