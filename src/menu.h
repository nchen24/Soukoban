#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "gameutils.h"

//TODO: Add support for when there are one or more empty places due to odd
//number of options

// Options will be printed left to right top to bottom, in the order they are
// given.
typedef struct Menu{
    char *message;
    char **options;
    bool **canGoTo;
    unsigned actualNumOps;
    UIntPair numOptions; // rows = y, cols = x
    Coord pointerLoc;
    int startRow;
} Menu;

// Public interface ==========================================================
                                                                            //
// Constructors                                                             //
extern Menu *makeMenu(const char * const msg, const char * const options[], //
                      unsigned opSize, UIntPair numOptions, int startRow);  //
                                                                            //
// Destructors                                                              //
extern void freeMenu(Menu *m);                                              //
                                                                            //
// Member functions                                                         //
extern int  getMenuInput(Menu *m);                                          //
                                                                            //
// ===========================================================================

// Internal functions ========================================================
                                                                            //
static inline void traverseMenu(Menu *m, Dir d);                            //
static inline void printMenu(Menu *m);                                      //
static inline int  convCoordTo1D(Coord c, UIntPair size);                   //
                                                                            //
// ===========================================================================

#endif
