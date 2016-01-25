#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include "gamemanager.h"
#include "game.h"
#include "menu.h"

extern void startGame(){
    for(;;){
    int mainMenuChoice = showMainMenu();
        switch(mainMenuChoice){
            case 0:
                playGame();
                break;
            case 1:;
                Game *g = makeGameFromSave(SAVE_PATH);
                if(g){
                    play(g);
                    freeGame(g);
                }
                break;
            case 2:
                // showCredits(); //TODO: Implement
                break;
            case 3:
                return;
        }
    }

    return;
}

static bool askTimeLimit(int startRow){
    showTitle();
    Menu *m = makeMenu(ASK_TIME_LIMIT, CONFIRM_OPS, 2, CONFIRM_MENU_FMT, startRow);
    bool timeLimit = !getMenuInput(m); // Yes is 0
    freeMenu(m);

    return timeLimit;
}

static bool askTurnLimit(int startRow){
    showTitle();
    Menu *m = makeMenu(ASK_TURN_LIMIT, CONFIRM_OPS, 2, CONFIRM_MENU_FMT, startRow);
    bool turnLimit = !getMenuInput(m);
    freeMenu(m);

    return turnLimit;
}

static glob_t getLevelList(){
    char cmd[1024]; // This MUST be 1025! 1024 WILL segfault!
    sprintf(cmd, "%s*%s", LEVEL_ROOT, LEVEL_FEX);

    glob_t globbuf;
    glob(cmd, 0, NULL, &globbuf);

    return globbuf;
}

static inline void playGame(){
    UIntPair bottomRightCorner = showTitle();

    glob_t ll           = getLevelList();
    UIntPair menuFmt    = {LEVEL_SELECT_COLS, ll.gl_matchc/LEVEL_SELECT_COLS};
    Menu *m             = makeMenu("", ll.gl_pathv, ll.gl_matchc, menuFmt, bottomRightCorner.y);
    int levelChoice     = getMenuInput(m);

    bool timeLimit      = askTimeLimit(bottomRightCorner.y);
    bool turnLimit      = askTurnLimit(bottomRightCorner.y);

    Game *g             = makeGame(ll.gl_pathv[levelChoice], timeLimit, turnLimit);
    play(g);

    freeGame(g);
    freeMenu(m);
    globfree(&ll);
}

static inline UIntPair showTitle(){
    clear();
    char buf[1024];
    FILE *fp = fopen(TITLE_PATH, "r");
    assert(fp);

    move(0, 0);
    UIntPair bottomRightCorner;
    while(fgets(buf, sizeof(buf)-1, fp)){
        printw(buf);
        bottomRightCorner.x = strlen(buf);
        bottomRightCorner.y++;
    }
    fclose(fp);
    refresh();
    return bottomRightCorner;
}

static inline int showMainMenu(){
    UIntPair bottomRightCorner = showTitle();
    UIntPair numOps = {MAIN_MENU_COLS,
                       sizeof(MENU_OPS) / sizeof(MENU_OPS[0])/MAIN_MENU_COLS}; 
    Menu *m = makeMenu("", MENU_OPS, numOps.y, numOps, bottomRightCorner.y);
    int mainMenuChoice = getMenuInput(m);
    freeMenu(m);
    return mainMenuChoice;
}
