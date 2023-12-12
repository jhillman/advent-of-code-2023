/* Day 10, part 1 = 6951 */

#include "tiles.h"

int main() {
    struct Tiles *tiles = getTiles();

    if (tiles) {
        int answer = 0;

        do {
            ++answer;

            tiles->stepper1 = updateStepper(tiles, tiles->stepper1);
            tiles->stepper2 = updateStepper(tiles, tiles->stepper2);
        } while (!(tiles->stepper1.current.x == tiles->stepper2.current.x && 
                   tiles->stepper1.current.y == tiles->stepper2.current.y));

        freeTiles(tiles);

        printf("%d", answer);
    }

    return 0;
}
