/* Day 19, part 1 = 399284 */

#include "parts.h"

int main() {
    struct SortingData *data = getSortingData();
    long answer = 0;

    if (data) {
        for (int i = 0; i < data->parts->count; i++) {
            answer += evaluatePart(data->workflows, "in", data->parts->data[i]);
        }

        freeSortingData(data);
    }

    printf("%ld", answer);

    return 0;
}
