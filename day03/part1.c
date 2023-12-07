/* Day 3, part 1 = 556057 */

#include "schematic.h"

int main() {
    struct Schematic *schematic = getSchematic();

    if (schematic) {
        int answer = 0;

        for (int y = 0; y < schematic->height; y++) {
            for (int x = 0; x < schematic->width; x++) {
                struct Part part = getPart(schematic, x, y);

                if (part.part) {
                    x += digitCount(part.number);
                    answer += part.number;
                }
            }
        }

        freeSchematic(schematic);

        printf("%d", answer);
    }

    return 0;
}
