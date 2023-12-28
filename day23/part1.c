/* Day 23, part 1 = 2310 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

struct Step {
    int x;
    int y;
};

bool equal(struct Step a, struct Step b) {
    return a.x == b.x && a.y == b.y;
}

struct Map {
    int width;
    int height;
    
    struct Step start;
    struct Step end;

    char **data;
    bool **visited;
};

void freeMap(struct Map *map) {
    for (int y = 0; y < map->height; y++) {
        free(map->data[y]);
        free(map->visited[y]);
    }

    free(map->data);
    free(map->visited);
    free(map);
}

int longestHike(struct Map *map, struct Step step, int hike) {
    if (!(step.x >= 0 && step.x < map->width && step.y >= 0 && step.y < map->height)) {
        return 0;
    }

    if (map->data[step.y][step.x] == '#') {
        return 0;
    }

    if (map->visited[step.y][step.x]) {
        return 0;
    }

    if (equal(step, map->end)) {
        return hike;
    }

    int longest = 0;
    int xDeltas[] = {-1, 0, 1, 0};
    int yDeltas[] = {0, 1, 0, -1};

    map->visited[step.y][step.x] = true;

    if (map->data[step.y][step.x] == '.') {
        for (int i = 0; i < 4; i++) {
            struct Step next = { step.x + xDeltas[i], step.y + yDeltas[i] };
            int length = longestHike(map, next, hike + 1);

            longest = max(longest, length);
        }
    } else {
        struct Step next = step;

        switch (map->data[next.y][next.x]) {
        case '^':
            --next.y;
            break;
        case 'v':
            ++next.y;
            break;
        case '<':
            --next.x;
            break;
        case '>':
            ++next.x;
            break;
        }

        longest = longestHike(map, next, hike + 1);
    }

    map->visited[step.y][step.x] = false;

    return longest;
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");
    int answer = 0;

    if (inputFile) {
        char c;

        struct Map *map = (struct Map *)calloc(1, sizeof(struct Map));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++map->height;
            }

            if (!map->height) {
                ++map->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        map->data = (char **)malloc(map->height * sizeof(char *));
        map->visited = (bool **)malloc(map->height * sizeof(bool *));

        for (int y = 0; y < map->height; y++) {
            map->data[y] = (char *)malloc(map->width * sizeof(char));
            map->visited[y] = (bool *)calloc(map->width, sizeof(bool));

            for (int x = 0; x < map->width; x++) {
                map->data[y][x] = fgetc(inputFile);

                if (y == 0 && map->data[y][x] == '.') {
                    map->start.x = x;
                    map->start.y = y;
                } else if (y == map->height - 1 && map->data[y][x] == '.') {
                    map->end.x = x;
                    map->end.y = y;
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        answer = longestHike(map, map->start, 0);

        freeMap(map);
    }

    printf("%d", answer);

    return 0;
}
