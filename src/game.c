#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include "game.h"
#include "menu.h"
#include "gameutils.h"


Game *makeGame(char *fileName, bool limitTime, bool limitTurns){
    Game *newGame = safemalloc(sizeof(Game));

    newGame->levelName = fileName;

    newGame->moveHistorySize = 1000;

    newGame->fieldHistory = safemalloc(sizeof(Field*) * newGame->moveHistorySize);
    newGame->fieldHistory[0] = makeFieldFromFile(fileName, &newGame->maxTime, &newGame->maxTurns); 
    newGame->playerPos = findPlayer(newGame->fieldHistory[0]);

    newGame->moveHistory = safemalloc(sizeof(char) + newGame->moveHistorySize);
    memset(newGame->moveHistory, 0, newGame->moveHistorySize);

    newGame->curPlace = 0;

    newGame->limitTurns = limitTurns;
    newGame->curTurn  = 0;
    newGame->maxTurns = 1000;

    newGame->limitTime = limitTime;
    newGame->startTime = time(NULL);
    newGame->maxTime   = 500;

    return newGame;
}

Game *makeGameFromSave(const char * const saveFileName){
    FILE *fp = fopen(saveFileName, "r");
    if(!fp){
        clear();
        UIntPair fmt = {1, 1};
        Menu *m = makeMenu(NO_SAVE_FILE, CONTINUE, 1, fmt, 0);
        getMenuInput(m);
        freeMenu(m);
        return NULL;
    }
    assert(fp);
    char s[256];
    fgets(s, sizeof(s), fp);
    s[strlen(s)-1] = '\0'; // Strip the newline from fgets

    char *levelName = s; // TODO: Error checking?

    Game *newGame = makeGame(levelName, false, false);

    memset(s, 0, sizeof(s));
    fgets(s, sizeof(s), fp);
    s[strlen(s)-1] = '\0'; // Strip the newline from fgets
    
    for(unsigned i = 0 ; i < strlen(s) ; i++){
        switch(s[i]){
            case 'W':
                movePlayer(newGame, WEST);
                break;
            case 'N':
                movePlayer(newGame, NORTH);
                break;
            case 'S':
                movePlayer(newGame, SOUTH);
                break;
            case 'E':
                movePlayer(newGame, EAST);
                break;
        }
    }

    return newGame;
}

void freeGame(Game *g){
    if(!g)
        return;
    for(int i = 0 ; i <= g->curPlace ; i++)
        freeField(g->fieldHistory[i]);
    free(g->fieldHistory);
    free(g->moveHistory);
    free(g);
}

extern void play(Game *g){
    clear();
    printControls(g);
    if(g->limitTurns)
        printMovesRemaining(g);
    printField(g->fieldHistory[g->curPlace]); 
    refresh();
    for(;;){
        if(g->limitTime)
            printTime(g);
        refresh();
        int input = getch();
        if(input == -1)
            continue;

        switch(input){
            case KEY_LEFT:
            case ALT_LEFT:
                movePlayer(g, WEST);
                break;
            case KEY_UP:
            case ALT_UP:
                movePlayer(g, NORTH);
                break;
            case KEY_DOWN:
            case ALT_DOWN:
                movePlayer(g, SOUTH);
                break;
            case KEY_RIGHT:
            case ALT_RIGHT:
                movePlayer(g, EAST);
                break;
            case RESET:
                reset(g);
                break;
            case UNDO:
                undo(g);
                break;
            case EXIT:; // Case cannot start w a declaration, ; is workaround
                Menu *m = makeMenu(ASK_SAVE_GAME, CONFIRM_OPS, 2, CONFIRM_MENU_FMT, 0);
                clear();
                if(!getMenuInput(m)) // Yes is 0
                    saveGame(g);
                freeMenu(m);
                return;
            default:
                //printf("%d", input);
                break;
        }
        if(checkWin(g)){
            showWin(g);
        } else {
            switch(checkGameOver(g)){
                case GO_TURNS:
                    printMessageToUser(g, GAME_OVER_TURN_MSG);
                    break;
                case GO_TIME:
                    printMessageToUser(g, GAME_OVER_TIME_MSG);
                    break;
                case GO_NONE:
                    break;
            }

        }
        if(g->limitTurns)
            printMovesRemaining(g);
        printField(g->fieldHistory[g->curPlace]); 
        //refresh();
    }
}

static bool movePlayer(Game *g, Dir d){
    const Field *curField = g->fieldHistory[g->curPlace];
    bool pushing = false;

    ObjType ot = peekAhead(curField, g->playerPos, d, 1);
    
    if(ot == WALL_T)
        return false;
    if(ot == BOX_T){
        if(peekAhead(curField, g->playerPos, d, 2) != NONE_T){
            return false;
        }
        pushing = true;
    }

    Field *newField = makeFieldFromField(curField);

    switch(d){
        case WEST:
            newField->fg[g->playerPos.y][g->playerPos.x] = EMPTY;
            newField->fg[g->playerPos.y][g->playerPos.x-1] = PLAYER;
            g->playerPos.x--;
            g->moveHistory[g->curPlace] = 'W';
            break;
        case SOUTH:
            newField->fg[g->playerPos.y][g->playerPos.x] = EMPTY;
            newField->fg[g->playerPos.y+1][g->playerPos.x] = PLAYER;
            g->playerPos.y++;
            g->moveHistory[g->curPlace] = 'S';
            break;
        case NORTH:
            newField->fg[g->playerPos.y][g->playerPos.x] = EMPTY;
            newField->fg[g->playerPos.y-1][g->playerPos.x] = PLAYER;
            g->playerPos.y--;
            g->moveHistory[g->curPlace] = 'N';
            break;
        case EAST:
            newField->fg[g->playerPos.y][g->playerPos.x] = EMPTY;
            newField->fg[g->playerPos.y][g->playerPos.x+1] = PLAYER;
            g->playerPos.x++;
            g->moveHistory[g->curPlace] = 'E';
            break;
        default:
            return false;
    }

    if(pushing){
        Coord oneAhead = nAheadInDir(g->playerPos, d, 1);
        newField->fg[oneAhead.y][oneAhead.x] = BOX;
    }

    g->curTurn++;
    appendNewFrame(g, newField);
    return true;
}

static void appendNewFrame(Game *g, Field *f){
    g->curPlace++;
    g->fieldHistory[g->curPlace] = f;
}

static void undo(Game *g){
    if(g->curPlace == 0)
        return;
    freeField(g->fieldHistory[g->curPlace]);
    g->moveHistory[g->curPlace] = '\0';
    g->curPlace--;
    g->playerPos = findPlayer(g->fieldHistory[g->curPlace]);
}

static void reset(Game *g){
    for(int i = 1 ; i <= g->curPlace ; i++)
        freeField(g->fieldHistory[i]);
 
    memset(g->moveHistory, 0, g->moveHistorySize);

    g->curPlace  = 0;
    g->curTurn   = 0;
    g->startTime = time(NULL);
    g->playerPos = findPlayer(g->fieldHistory[g->curPlace]);
}

static bool checkWin(Game *g){
    Field *curField = g->fieldHistory[g->curPlace];
    for(unsigned r = 0 ; r < curField->size.y ; r++){
        for(unsigned c = 0 ; c < curField->size.x ; c++){
            if(curField->bg[r][c] == GOAL){
                if(curField->fg[r][c] != BOX){
                    return false;
                }
            }
        }
    }
    return true;
}

static void showWin(Game *g){
    g->fieldHistory[g->curPlace]->fg[g->playerPos.y][g->playerPos.x] = WINCHAR;
    printMessageToUser(g, LEVEL_CLEAR_MSG);
}

static GameOverCode checkGameOver(Game *g){
    if(g->limitTurns && g->curTurn >= g->maxTurns)
        return GO_TURNS;
    if(g->limitTime && time(NULL) - g->startTime > g->maxTime)
        return GO_TIME;
    return GO_NONE;
}

static void printControls(Game *g){
    char str[] = "(U)ndo (R)estart (\\) Quit\n";
    mvprintw(g->fieldHistory[0]->size.y, 0, "%s", str);
}

static void printMovesRemaining(Game *g){
    char str[] = "Moves remaining:";
    mvprintw(g->fieldHistory[0]->size.y+1, 0, "%s %d  ", str, g->maxTurns - g->curTurn);
}
static void printTime(Game *g){
    char str[] = "Time:";
    mvprintw(g->fieldHistory[0]->size.y+2, 0, "%s %d   ", str, time(NULL) - g->startTime);
}
static void printMessageToUser(Game *g, const char msg[]){
    mvprintw(g->fieldHistory[0]->size.y+3, 0, "%s                                  ", msg);
}

static void saveGame(Game *g){
    FILE *fp = fopen(SAVE_PATH, "w");
    fprintf(fp, "%s\n%s\n", g->levelName, g->moveHistory);
}

static Coord findPlayer(Field *f){
    for(unsigned r = 0 ; r < f->size.y ; r++){
        for(unsigned c = 0 ; c < f->size.x ; c++){
            if(f->fg[r][c] == PLAYER || f->fg[r][c] == PLAYER_O){
                Coord coord;
                coord.x = c ; coord.y = r;
                return coord;
            }
        }
    }
    assert(0);
}
