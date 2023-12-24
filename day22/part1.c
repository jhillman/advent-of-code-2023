/* Day 22, part 1 = 527 */

#include "tower.h"

int main() {
    struct Tower *tower = getTower();
    int answer = 0;

    if (tower) {
        struct Brick *brick;

        for (int i = 0; i < tower->bricks->count; i++) {
            brick = &tower->bricks->data[i];

            bool supportsNoBricks = brick->supporting->count == 0;
            bool supportedBricksHaveOtherSupports = true;

            for (int j = 0; supportedBricksHaveOtherSupports && j < brick->supporting->count; j++) {
                supportedBricksHaveOtherSupports = supportedBricksHaveOtherSupports && 
                    brick->supporting->data[j].supportedBy->count > 1;
            }

            if (supportsNoBricks || supportedBricksHaveOtherSupports) {
                ++answer;
            }
        }

        freeTower(tower);
    }

    printf("%d", answer);

    return 0;
}
