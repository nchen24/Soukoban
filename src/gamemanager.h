#ifndef GAME_MANAGER_H_INCLUDED
#define GAME_MANAGER_H_INCLUDED
#include <stdbool.h>
#include <glob.h>
#include "gameutils.h"

typedef struct GameManager{


} GameManager;

// Public interface ==========================================================
                                                                            //
extern void startGame();                                                    //
                                                                            //
// ===========================================================================

// Internal functions ========================================================
                                                                            //
static inline bool askTimeLimit(int startRow);                              //
static inline bool askTurnLimit(int startRow);                              //
static inline void playGame();                                              //
static inline glob_t getLevelList();                                        //
static inline UIntPair showTitle();                                         //
static inline int showMainMenu();                                           //
                                                                            //
// ===========================================================================

#endif
