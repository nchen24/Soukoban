#include <string.h>
#include <stdlib.h>
#include "gameutils.h"

extern void *safemalloc(size_t size){
    void *p = malloc(size);
    assert(p);
    return p;
}

extern char *safestrclone(const char * const s){
    int strSize = strlen(s) + 1;
    char * ret = safemalloc(sizeof(char) * strSize);
    strlcpy(ret, s, strSize);
    return ret;
}

extern Coord nAheadInDir(Coord c, Dir d, int n){
    Coord newCoord = c;
    switch(d){
        case WEST:  newCoord.x -= n; break;
        case SOUTH: newCoord.y += n; break;
        case NORTH: newCoord.y -= n; break;
        case EAST:  newCoord.x += n; break;
    }
    return newCoord;
}
