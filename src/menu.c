#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <curses.h>
#include "menu.h"

extern Menu *makeMenu(const char * const msg, const char * const options[],
                      unsigned opSize, UIntPair numOptions, int startRow){

    Menu *newMenu    = safemalloc(sizeof(Menu));
    newMenu->message = strndup(msg, STR_MAX_LEN);
    newMenu->options = safemalloc(sizeof(char *) * opSize);
    for(unsigned i = 0 ; i < opSize ; i ++){
        newMenu->options[i] = strdup(options[i]);
    }
    
    newMenu->canGoTo = safemalloc(sizeof(bool *) * numOptions.y);
    for(unsigned i = 0 ; i < numOptions.y ; i++)
        newMenu->canGoTo[i] = safemalloc(sizeof(bool) * numOptions.x);
    for(unsigned r = 0 ; r < numOptions.y ; r++)
        for(unsigned c = 0 ; c < numOptions.x ; c++)
            newMenu->canGoTo[r][c] = false;

    newMenu->actualNumOps   = opSize;
    newMenu->numOptions     = numOptions;
    newMenu->pointerLoc.x   = 0;
    newMenu->pointerLoc.y   = 0;
    newMenu->startRow       = startRow;

    return newMenu;
}

extern void freeMenu(Menu *m){
    if(!m)
        return;
    free(m->message);

    for(unsigned i = 0 ; i < m->actualNumOps ; i++)
        free(m->options[i]);
    free(m->options);

    for(unsigned i = 0 ; i < m->numOptions.y ; i++)
        free(m->canGoTo[i]);
    free(m->canGoTo);

    free(m);
}

extern int getMenuInput(Menu *m){
    //clear();
    printMenu(m);
    int input;
    for(;;){
        input = getch();
        if(input == -1)
            continue;

        switch(input){
            case KEY_LEFT:
            case ALT_LEFT:
                traverseMenu(m, WEST);
                break;
            case KEY_UP:
            case ALT_UP:
                traverseMenu(m, NORTH);
                break;
            case KEY_DOWN:
            case ALT_DOWN:
                traverseMenu(m, SOUTH);
                break;
            case KEY_RIGHT:
            case ALT_RIGHT:
                traverseMenu(m, EAST);
                break;
            case SELECT:
            case ALT_SELECT:
                return convCoordTo1D(m->pointerLoc, m->numOptions);
            case EXIT:
                return -1;
            default:
                //printf("%d", input);
                break;
        }
        printMenu(m);
        refresh();
    }
}
static void traverseMenu(Menu *m, Dir d){
    switch(d){
        case WEST:
            if(m->pointerLoc.x > 0)
                m->pointerLoc.x--;
            break;
        case SOUTH:
            if(m->pointerLoc.y < m->numOptions.y-1)
                m->pointerLoc.y++;
            break;
        case NORTH:
            if(m->pointerLoc.y > 0)
                m->pointerLoc.y--;
            break;
        case EAST:
            if(m->pointerLoc.x < m->numOptions.x-1)
                m->pointerLoc.x++;
            break;
    }
}

static void printMenu(Menu *m){
    unsigned counter = 0;
    mvprintw(m->startRow+1, 0, "%s\n", m->message);
    for(unsigned r = 0 ; r < m->numOptions.y ; r++){
        if(counter >= m->actualNumOps)
            return;
        for(unsigned c = 0 ; c < m-> numOptions.x ; c++){
            Coord co = {c, r};
            if(r == m->pointerLoc.y && c == m->pointerLoc.x)
                printw("    > %s <", m->options[convCoordTo1D(co, m->numOptions)]);
            else
                printw("      %s  ", m->options[convCoordTo1D(co, m->numOptions)]);
        }
        printw("\n");
        counter++;
        //printf("%u", counter);
    }
}


static int convCoordTo1D(Coord c, UIntPair size){
    return (c.y * size.x) + c.x;
}
