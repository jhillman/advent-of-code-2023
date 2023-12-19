/* Day 17, part 2 = 1073 */

#include "crucible.h"

int main() {
    struct HeatLossData *data = getHeatLossData();

    if (data) {
        int heatLoss = leastHeatLoss(data, 4, 10);

        freeHeatLossData(data);

        printf("%d", heatLoss);
    }

    return 0;
}