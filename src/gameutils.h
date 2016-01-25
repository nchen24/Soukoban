#ifndef GAME_UTILS_H_INCLUDED
#define GAME_UTILS_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#define DEBUG 1
//#NDEBUG // Disables assert

// Utility types and enums
typedef enum Dir {WEST, SOUTH, NORTH, EAST} Dir;
typedef enum ObjType {WALL_T, GOAL_T, BOX_T, PLAYER_T, NONE_T, ERR_T} ObjType; // Add BOX_O, PLAYER_O?
typedef enum GameOverCode {GO_TIME, GO_TURNS, GO_NONE} GameOverCode;

typedef struct UIntPair{
    unsigned x;
    unsigned y;
} UIntPair;
typedef UIntPair Coord;

extern void *safemalloc(size_t size);
extern char *safestrclone(const char * const s);
extern Coord nAheadInDir(Coord c, Dir d, int n);

// Field characters
static const char WALL                  = '#';
static const char GOAL                  = '*';
static const char BOX                   = 'o';
static const char BOX_O                 = 'O';
static const char PLAYER                = 'p';
static const char PLAYER_O              = 'P';
static const char EMPTY                 = ' ';
static const char WINCHAR               = '!';

// Controls
static const int ALT_LEFT               = 'h';
static const int ALT_DOWN               = 'j';
static const int ALT_UP                 = 'k';
static const int ALT_RIGHT              = 'l';
static const int RESET                  = 'r';
static const int UNDO                   = 'u';
static const int EXIT                   = '\\';
static const int SELECT                 = '\n';
static const int ALT_SELECT             = ' ';

// Misc game constants
static const int MAIN_MENU_COLS         = 1;
static const int LEVEL_SELECT_COLS      = 2;
static const int STR_MAX_LEN            = 80;
static const UIntPair CONFIRM_MENU_FMT  = {1, 2};

// Game strings
static const char * const MENU_OPS[]    = {"New Game", "Resume", "Credits", "Exit"};
static const char * const CONFIRM_OPS[] = {"Yes", "No"};
static const char * const CONTINUE[]    = {"Ok"};

static const char NO_SAVE_FILE[]        = "Error: No save file found.";

static const char ASK_TIME_LIMIT[]      = "Enable time limit?";
static const char ASK_TURN_LIMIT[]      = "Enable turn limit?";
static const char ASK_SAVE_GAME[]       = "Would you like to save the game?";

static const char GAME_OVER_TIME_MSG[]  = "Game over, time up!";
static const char GAME_OVER_TURN_MSG[]  = "Game over, out of turns!";
static const char LEVEL_CLEAR_MSG[]     = "Congratulations, you win!";

// Level paths
static const char LEVEL_ROOT[]          = "../levels/";
static const char LEVEL_FEX[]           = ".lvl";
static const char TITLE_PATH[]          = "../title.txt";
static const char SAVE_PATH[]           = "level.sav";

#endif

