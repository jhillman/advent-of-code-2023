/* Day 4, part 1 = 25651 */

#include "cards.h"
#include <math.h>

int main() {
    struct Cards *cards = getCards();

    if (cards) {
        int answer = 0;

        for (int i = 0; i < cards->count; i++) {
            if (cards->cards[i].winners) {
                answer += (int)pow(2, cards->cards[i].winners - 1);
            }
        }

        freeCards(cards);

        printf("%d", answer);
    }

    return 0;
}