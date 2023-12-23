/* Day 22, part 1 = 527 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

struct Coordinates {
    int x;
    int y;
    int z;
};

struct CoordinatesList {
    int capacity;
    int count;
    struct Coordinates *data;
};

bool coordinatesEqual(struct Coordinates first, struct Coordinates second) {
    return first.x == second.x && first.y == second.y && first.z == second.z;
}

void addCoordinates(struct CoordinatesList *coordinatesList, struct Coordinates coordinates) {
    if (coordinatesList->count == coordinatesList->capacity) {
        coordinatesList->capacity += 10;
        coordinatesList->data = (struct Coordinates *)realloc(coordinatesList->data, coordinatesList->capacity * sizeof(struct Coordinates));
    }

    coordinatesList->data[coordinatesList->count++] = coordinates;
}

struct Bricks;

struct Brick {
    int index;
    struct Coordinates start;
    struct Coordinates end;
    
    struct CoordinatesList *coordinates;

    struct Bricks *supporting;
    struct Bricks *supportedBy;
};

struct Bricks {
    int capacity;
    int count;
    struct Brick *data;
};

struct Brick makeBrick(struct Coordinates start, struct Coordinates end) {
    struct Brick brick = { -1,
        start, end, 
        calloc(1, sizeof(struct CoordinatesList)),
        calloc(1, sizeof(struct Bricks)),
        calloc(1, sizeof(struct Bricks))
     };

    for (int x = start.x; x <= end.x; x++) {
        for (int y = start.y; y <= end.y; y++) {
            for (int z = start.z; z <= end.z; z++) {
                addCoordinates(brick.coordinates, (struct Coordinates){ x, y, z });
            }
        }
    }

    return brick;
}

bool bricksEqual(struct Brick first, struct Brick second) {
    return coordinatesEqual(first.start, second.start) && coordinatesEqual(first.end, second.end);
}

void addBrick(struct Bricks *bricks, struct Brick brick) {
    if (bricks->count == bricks->capacity) {
        bricks->capacity += 10;
        bricks->data = (struct Brick *)realloc(bricks->data, bricks->capacity * sizeof(struct Brick));
    }

    bricks->data[bricks->count++] = brick;
}

int compareBricks(const void *a, const void *b) {
    return ((struct Brick *)a)->start.z - ((struct Brick *)b)->start.z; 
}

struct Tower {
    int height;
    int width;
    int depth;

    bool ***data;
};

void addBrickToTower(struct Tower *tower, struct Brick brick) {
    struct Coordinates coordinates;

    for (int i = 0; i < brick.coordinates->count; i++) {
        coordinates = brick.coordinates->data[i];

        tower->data[coordinates.y][coordinates.x][coordinates.z] = true;
    }
}

bool towerContainsBrick(struct Tower *tower, struct Brick brick) {
    struct Coordinates coordinates;
    bool contains = false;

    for (int i = 0; !contains && i < brick.coordinates->count; i++) {
        coordinates = brick.coordinates->data[i];

        contains = tower->data[coordinates.y][coordinates.x][coordinates.z];
    }

    return contains;
}

bool letBricksFall(struct Bricks *bricks, struct Tower *tower, bool bailEarly) {
    struct Brick brick;
    struct Brick droppedBrick;
    bool bricksFell = false;

    for (int i = 0; i < bricks->count; i++) {
        brick = bricks->data[i];

        while (true) {
            droppedBrick = makeBrick((struct Coordinates){ brick.start.x, brick.start.y, brick.start.z - 1 }, 
                                     (struct Coordinates){ brick.end.x, brick.end.y, brick.end.z - 1 });

            if (droppedBrick.start.z > 0 && !towerContainsBrick(tower, droppedBrick)) {
                free(brick.coordinates->data);
                free(brick.coordinates);

                brick = droppedBrick;

                bricksFell = true;

                if (bailEarly) {
                    break;
                }
            } else {
                free(droppedBrick.coordinates->data);
                free(droppedBrick.coordinates);

                break;
            }
        }

        bricks->data[i] = brick;

        addBrickToTower(tower, brick);
    }

    return bricksFell;
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        struct Bricks *bricks = (struct Bricks *)calloc(1, sizeof(struct Bricks));
        struct Coordinates start;
        struct Coordinates end;
        struct Coordinates max = { 0, 0, 0 };
        struct Tower *tower = (struct Tower *)calloc(1, sizeof(struct Tower));
        struct Brick *brick;
        struct Brick *other;
        int top = 0;
        int answer = 0;

        while (fscanf(inputFile, "%d,%d,%d~%d,%d,%d", &start.x, &start.y, &start.z, &end.x, &end.y, &end.z) == 6) {
            max.x = max(max.x, max(start.x, end.x));
            max.y = max(max.y, max(start.y, end.y));
            max.z = max(max.z, max(start.z, end.z));

            addBrick(bricks, makeBrick(start, end));
        }

        fclose(inputFile);

        qsort(bricks->data, bricks->count, sizeof(struct Brick), compareBricks);

        tower->width = max.x + 1;
        tower->height = max.y + 1;
        tower->depth = max.z + 1;

        tower->data = (bool ***)malloc(tower->height * sizeof(bool **));

        for (int y = 0; y < tower->height; y++) {
            tower->data[y] = (bool **)malloc(tower->width * sizeof(bool *));

            for (int x = 0; x < tower->width; x++) {
                tower->data[y][x] = (bool *)calloc(tower->depth, sizeof(bool));
            }
        }

        letBricksFall(bricks, tower, false);

        qsort(bricks->data, bricks->count, sizeof(struct Brick), compareBricks);

        for (int i = 0; i < bricks->count; i++) {
            brick = &bricks->data[i];
            brick->index = i;

            for (int j = i + 1; j < bricks->count; j++) {
                other = &bricks->data[j];
                other->index = j;

                if (brick->end.z < other->start.z - 1) {
                    continue;
                } else if (brick->end.z == other->start.z - 1 && 
                          (max(brick->start.x, other->start.x) <= min(brick->end.x, other->end.x) && 
                           max(brick->start.y, other->start.y) <= min(brick->end.y, other->end.y))) {
                    addBrick(other->supportedBy, *brick);
                    addBrick(brick->supporting, *other);
                }
            }
        }

        for (int i = 0; i < bricks->count; i++) {
            brick = &bricks->data[i];

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

        for (int i = 0; i < bricks->count; i++) {
            free(bricks->data[i].coordinates->data);
            free(bricks->data[i].coordinates);
            free(bricks->data[i].supporting->data);
            free(bricks->data[i].supporting);
            free(bricks->data[i].supportedBy->data);
            free(bricks->data[i].supportedBy);
        }

        free(bricks->data);
        free(bricks);

        for (int y = 0; y < tower->height; y++) {
            for (int x = 0; x < tower->width; x++) {
                free(tower->data[y][x]);
            }

            free(tower->data[y]);
        }

        free(tower->data);
        free(tower);

        printf("%d", answer);
    }

    return 0;
}
