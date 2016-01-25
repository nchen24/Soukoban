#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <curses.h>
#include "field.h"

Field *makeFieldFromFile(char *fileName, time_t *time, int *turns){
    Field *newField = allocateField(getBoardDims(fileName));
    parseAndFill(newField, fileName);

    return newField;
}

extern Field *makeFieldFromField(const Field *f){
    Field *newField = allocateField(f->size);
    newField->size = f->size;
    for(unsigned r = 0 ; r < newField->size.y ; r++){
        for(unsigned c = 0 ; c < newField->size.x ; c++){
            newField->fg[r][c] = f->fg[r][c];
            newField->bg[r][c] = f->bg[r][c];
        }
    }
    return newField;
}

void freeField(Field *f){
    if(!f)
        return;
    for(unsigned i = 0 ; i < f->size.y ; i++){
        free(f->fg[i]);
        free(f->bg[i]);
    }
    free(f->fg);
    free(f->bg);
    free(f);
}

extern void printField(const Field *f){
    char fgc, bgc, ctp; // fg char, bg char, char to print
    for(unsigned r = 0 ; r < f->size.y ; r++){
        for(unsigned c = 0 ; c < f->size.x ; c++){
            fgc = f->fg[r][c];
            bgc = f->bg[r][c];
            if(bgc == GOAL){
                if(fgc == PLAYER)
                    ctp = PLAYER_O;
                else if(fgc == BOX)
                    ctp = BOX_O;
                else
                    ctp = GOAL;
            }
            else {
                ctp = fgc == EMPTY ? bgc : fgc;
            }
            mvaddch(r, c, ctp);
        }
    }
}

extern ObjType objTypeAtCoord(const Field *f, Coord c){
    switch(f->fg[c.y][c.x]){
        case PLAYER:
            return PLAYER_T;
        case BOX:
        case BOX_O:
            return BOX_T;
    }
    switch(f->bg[c.y][c.x]){
        case WALL:
            return WALL_T;
    }
    return NONE_T;
}

extern ObjType peekAhead(const Field *f, Coord c, Dir d, unsigned n){
    Coord newCoord = nAheadInDir(c, d, n);
    if(newCoord.x >= f->size.x || newCoord.y >= f->size.y)
        return ERR_T;
    return objTypeAtCoord(f, newCoord);
}

static Field *allocateField(UIntPair size){
    Field *newField = safemalloc(sizeof(Field));
    newField->size = size;
    newField->fg = safemalloc(sizeof(char*) * newField->size.y);
    newField->bg = safemalloc(sizeof(char*) * newField->size.y);
    for(unsigned i = 0 ; i < newField->size.y ; i++){
        newField->fg[i] = safemalloc(sizeof(char) * newField->size.x);
        newField->bg[i] = safemalloc(sizeof(char) * newField->size.x);
    }

    for(unsigned r = 0 ; r < newField->size.y ; r++){
        for(unsigned c = 0 ; c < newField->size.x ; c++){
            newField->fg[r][c] = newField->bg[r][c] = EMPTY;
        }
    }
    return newField;
}
static void parseAndFill(Field *f, char *fileName){
    FILE *fp = fopen(fileName, "r");
    assert(fp);
    char s[256];
    fgets(s, sizeof(s), fp);
    unsigned rowCounter = 0; 
    unsigned colCounter = 0; 
    char c;
    while((c = fgetc(fp)) != -1){//!feof(fp)){
        assert(isValidBoardChar(c));
        switch(c){
            case WALL: 
            case GOAL:
                f->bg[rowCounter][colCounter] = c;
                break;
            case BOX:
            case PLAYER:
                f->fg[rowCounter][colCounter] = c;
                break;
            case BOX_O:
                f->bg[rowCounter][colCounter] = GOAL;
                f->fg[rowCounter][colCounter] = BOX;
                break;
            case PLAYER_O:
                f->bg[rowCounter][colCounter] = GOAL;
                f->fg[rowCounter][colCounter] = PLAYER;
                break;
            case '\n':
            case ' ':
                break;
            default:
                break;
        }
        if(c != '\n'){
            colCounter++;
            if(colCounter == f->size.x){
                colCounter = 0;
                rowCounter++;
            }
            if(colCounter > f->size.y)
                break;
        }
    }
    fclose(fp);
}

static UIntPair getBoardDims(char *levelName){
    FILE *fp;
    fp = fopen(levelName, "r");
    assert(fp);
    UIntPair size;
    fscanf(fp, "%d %d", &size.y, &size.x);
    fclose(fp);
    return size;
}

static bool isValidBoardChar(char c){
    switch(c){
        case WALL:      return true;
        case GOAL:      return true;
        case BOX:       return true;
        case BOX_O:     return true;
        case PLAYER:    return true;
        case PLAYER_O:  return true;
        case EMPTY:     return true;
        case '\n':      return true;
        default:        printf("'%d'", (int)c);return false;
    }
}
