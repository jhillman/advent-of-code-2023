/* Day 21, part 1 = 3585 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STEPS 64

struct Farm {
    int width;
    int height;
    char **tiles;
    bool ***visited;
};

void step(struct Farm *farm, int x, int y, int steps) {
    int xDeltas[] = {-1, 0, 1, 0};
    int yDeltas[] = {0, 1, 0, -1};

    farm->visited[y][x][steps] = true;

    if (steps == STEPS) {
        farm->tiles[y][x] = 'O';
    } else {
        for (int i = 0; i < 4; i++) {
            int stepX = x + xDeltas[i];
            int stepY = y + yDeltas[i];

            if (!(stepX >= 0 && stepX < farm->width && stepY >= 0 && stepY < farm->height)) {
                continue;
            }

            if (farm->tiles[stepY][stepX] == '#' || farm->visited[stepY][stepX][steps + 1]) {
                continue;
            }

            step(farm, stepX, stepY, steps + 1);
        }
    }
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char c;
        struct Farm *farm = (struct Farm *)calloc(1, sizeof(struct Farm));
        int startX;
        int startY;
        int answer = 0;

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++farm->height;
            }

            if (!farm->height) {
                ++farm->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        farm->tiles = (char **)malloc(farm->height * sizeof(char *));
        farm->visited = (bool ***)malloc(farm->height * sizeof(bool **));

        for (int y = 0; y < farm->height; y++) {
            farm->tiles[y] = (char *)malloc(farm->width * sizeof(char));
            farm->visited[y] = (bool **)malloc(farm->width * sizeof(bool *));

            for (int x = 0; x < farm->width; x++) {
                farm->tiles[y][x] = fgetc(inputFile);
                farm->visited[y][x] = (bool *)calloc(STEPS + 1, sizeof(bool));

                if (farm->tiles[y][x] == 'S') {
                    startX = x;
                    startY = y;
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        step(farm, startX, startY, 0);

        for (int y = 0; y < farm->height; y++) {
            for (int x = 0; x < farm->width; x++) {
                free(farm->visited[y][x]);

                answer += farm->tiles[y][x] == 'O' ? 1 : 0;
            }

            free(farm->tiles[y]);
            free(farm->visited[y]);
        }

        free(farm->tiles);
        free(farm->visited);
        free(farm);

        printf("%d", answer);
    }

    return 0;
}
