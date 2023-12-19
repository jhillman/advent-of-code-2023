/* Day 17, part 1 = 902 */

#include "crucible.h"

int main() {
    struct HeatLossData *data = getHeatLossData();

    if (data) {
        int heatLoss = leastHeatLoss(data, 0, 3);

        freeHeatLossData(data);

        printf("%d", heatLoss);
    }

    return 0;
}