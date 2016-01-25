#ifndef GAME_INCLUDED_H
#define GAME_INCLUDED_H
#include <time.h>
#include "field.h"

typedef struct Game{
    char *levelName;

    int moveHistorySize;
    Field **fieldHistory;
    char *moveHistory;

    int curPlace;

    bool limitTurns;
    int curTurn;
    int maxTurns;

    bool limitTime;
    time_t startTime;
    time_t maxTime;

    Coord playerPos;
} Game;

// Public interface ==========================================================
                                                                            //
// Constructors                                                             //
extern Game *makeGame(char *fileName, bool limitTime, bool limitTurns);     //
extern Game *makeGameFromSave(const char * const saveFileName);             //
                                                                            //
// Destructors                                                              //
extern void freeGame(Game *g);                                              //
                                                                            //
// Member functions                                                         //
extern void play(Game *g);                                                  //
                                                                            //
// ===========================================================================

// Internal functions ========================================================
                                                                            //
static inline bool movePlayer(Game *g, Dir d);                              //
static inline void appendNewFrame(Game *g, Field *f);                       //
                                                                            //
static inline void undo(Game *g);                                           //
static inline void reset(Game *g);                                          //
                                                                            //
static inline bool checkWin(Game *g);                                       //
static inline void showWin(Game *g);                                        //
static inline GameOverCode checkGameOver(Game *g);                          //
                                                                            //
static inline void printControls(Game *g);                                  //
static inline void printMovesRemaining(Game *g);                            //
static inline void printTime(Game *g);                                      //
static inline void printMessageToUser(Game *g, const char msg[]);           //
static inline void saveGame(Game *g);                                       //
                                                                            //
static inline Coord findPlayer(Field *f);                                   //
                                                                            //
// ===========================================================================

#endif

