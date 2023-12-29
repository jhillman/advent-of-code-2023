#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

struct Location {
    int x;
    int y;
};

struct Locations {
    int capacity;
    int count;
    struct Location *data;
};

void addLocation(struct Locations *locations, struct Location location) {
    if (locations->count == locations->capacity) {
        locations->capacity += 10;
        locations->data = (struct Location *)realloc(locations->data, locations->capacity * sizeof(struct Location));
    }

    locations->data[locations->count++] = location;
}

bool equal(struct Location a, struct Location b) {
    return a.x == b.x && a.y == b.y;
}

struct IntersectionDistance {
    long index;
    int distance;
};

struct IntersectionDistances {
    int count;
    struct IntersectionDistance *data;
};

struct IntersectionData {
    int count;
    struct IntersectionDistances **distances;
};

struct Map {
    int width;
    int height;
    
    struct Location start;
    struct Location end;

    char **data;
    bool **visited;

    struct IntersectionData *intersections;
    int **intersectionLookup;
};

enum HikeType {
    ICY,
    DRY
};

void freeMap(struct Map *map) {
    for (int y = 0; y < map->height; y++) {
        free(map->data[y]);
        free(map->visited[y]);
    }

    free(map->data);
    free(map->visited);

    if (map->intersections) {
        for (int i = 0; i < map->intersections->count; i++) {
            free(map->intersections->distances[i]->data);
            free(map->intersections->distances[i]);
        }

        free(map->intersections->distances);

        for (int y = 0; y < map->height; y++) {
            free(map->intersectionLookup[y]);
        }

        free(map->intersectionLookup);
        free(map->intersections);
    }

    free(map);
}

int longestIcyHike(struct Map *map, struct Location location, int hikeLength) {
    int longest = 0;

    if (!(location.x >= 0 && location.x < map->width && location.y >= 0 && location.y < map->height)) {
        return 0;
    }

    if (map->data[location.y][location.x] == '#') {
        return 0;
    }

    if (map->visited[location.y][location.x]) {
        return 0;
    }

    if (equal(location, map->end)) {
        return hikeLength;
    }

    int xDeltas[] = {-1, 0, 1, 0};
    int yDeltas[] = {0, 1, 0, -1};

    map->visited[location.y][location.x] = true;

    if (map->data[location.y][location.x] == '.') {
        for (int i = 0; i < 4; i++) {
            struct Location next = { location.x + xDeltas[i], location.y + yDeltas[i] };
            int length = longestIcyHike(map, next, hikeLength + 1);

            longest = max(longest, length);
        }
    } else {
        struct Location next = location;

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

        longest = longestIcyHike(map, next, hikeLength + 1);
    }

    map->visited[location.y][location.x] = false;

    return longest;
}

void addIntersection(struct Map *map, struct Location parent, struct Location location, int pathLength) {
    struct Location locations[] = { parent, location };

    for (int i = 0; i < 2; i++) {
        struct Location location = locations[i];

        if (map->intersectionLookup[location.y][location.x] == -1) {
            map->intersectionLookup[location.y][location.x] = map->intersections->count;

            map->intersections->distances = (struct IntersectionDistances **)realloc(map->intersections->distances, (map->intersections->count + 1) * sizeof(struct IntersectionDistances *));
            map->intersections->distances[map->intersections->count++] = (struct IntersectionDistances *)calloc(1, sizeof(struct IntersectionDistances *));
        }
    }

    for (int i = 0; i < 2; i++) {
        struct Location first = i == 0 ? parent : location;
        struct Location second = i == 0 ? location : parent;
        struct IntersectionDistances *distances = map->intersections->distances[map->intersectionLookup[first.y][first.x]];

        distances->data = (struct IntersectionDistance *)realloc(distances->data, (distances->count + 1) * sizeof(struct IntersectionDistance));
        distances->data[distances->count++] = (struct IntersectionDistance){ map->intersectionLookup[second.y][second.x], pathLength };
    }
}

void findIntersections(struct Map *map, struct Location parent, struct Location location) {
    int pathLength = equal(location, parent) ? 0 : 1;
    int yDeltas[] = {-1, 0, 1, 0};
    int xDeltas[] = {0, 1, 0, -1};
    struct Locations *steps = (struct Locations *)calloc(1, sizeof(struct Locations));

    while (true) {
        map->data[location.y][location.x] = ' ';
        ++pathLength;

        if (equal(location, map->end)) {
            addIntersection(map, parent, location, pathLength - 1);
        }

        steps->count = 0;

        for (int i = 0; i < 4; i++) {
            struct Location next = { location.x + xDeltas[i], location.y + yDeltas[i] };

            if (!(next.x >= 0 && next.x < map->width && next.y >= 0 && next.y < map->height)) {
                continue;
            }

            if (equal(next, parent)) {
                continue;
            }

            if (map->data[next.y][next.x] == '.') {
                addLocation(steps, next);
            }

            if (map->intersectionLookup[next.y][next.x] > -1) {
                addIntersection(map, parent, next, pathLength);
            }
        }

        if (steps->count == 1) {
            location = *steps->data;
        } else if (steps->count > 0) {
            addIntersection(map, parent, location, pathLength - 1);

            for (int i = 0; i < steps->count; i++) {
                findIntersections(map, location, steps->data[i]);
            }
        } else {
            break;
        }
    }

    free(steps->data);
    free(steps);
}

int longestDryHike(struct Map *map, long index, long endIndex, int hikeLength) {
    if (index == endIndex) {
        return hikeLength;
    }

    *map->visited[index] = true;

    int longest = 0;

    struct IntersectionDistances *distances = map->intersections->distances[index];

    for (int i = 0; i < distances->count; i++) {
        struct IntersectionDistance path = distances->data[i];

        if (*map->visited[path.index]) {
            continue;
        }

        int length = longestDryHike(map, path.index, endIndex, hikeLength + path.distance);

        longest = max(longest, length);
    }

    *map->visited[index] = false;

    return longest;
}

int longestHike(struct Map *map, enum HikeType type) {
    int longest = 0;

    if (type == DRY) {
        map->intersections = (struct IntersectionData *)calloc(1, sizeof(struct IntersectionData));
        map->intersectionLookup = (int **)malloc(map->height * sizeof(int *));

        for (int y = 0; y < map->height; y++) {
            map->intersectionLookup[y] = (int *)malloc(map->width * sizeof(int));

            for (int x = 0; x < map->width; x++) {
                map->intersectionLookup[y][x] = -1;

                if (map->data[y][x] != '#' && map->data[y][x] != '.') {
                    map->data[y][x] = '.';
                }
            }
        }

        findIntersections(map, map->start, map->start);

        int startIndex = map->intersectionLookup[map->start.y][map->start.x];
        int endIndex = map->intersectionLookup[map->end.y][map->end.x];
        struct IntersectionDistance lastPath = *map->intersections->distances[endIndex]->data;

        endIndex = lastPath.index;

        longest = longestDryHike(map, startIndex, endIndex, lastPath.distance);
    } else if (type == ICY) {
        longest = longestIcyHike(map, map->start, 0);
    }

    return longest;    
}

struct Map *getMap() {
    FILE *inputFile = fopen("input.txt", "r");
    struct Map *map = NULL;

    if (inputFile) {
        map = (struct Map *)calloc(1, sizeof(struct Map));
        char c;

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
    }

    return map;
}
