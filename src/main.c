#include <stdio.h>
#include <assert.h>
#include <curses.h>
#include "gamemanager.h"
#include "game.h"
#include "field.h"

void setupNCurses();

int main(int argc, char *argv[]){
    if(argc!=1){
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    setupNCurses();

    //printf("\e]12;%s\a", "white");
    //fflush(stdout);


    startGame();  

    endwin();
    return 0;
}

void setupNCurses(){
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    //raw();
    //nonl();
}
