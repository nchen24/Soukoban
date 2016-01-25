#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <time.h>
#include "gameutils.h"
typedef struct Field{
    UIntPair size;
    char **fg;
    char **bg;
} Field;

// Public interface ==========================================================
                                                                            //
// Constructors                                                             //
extern Field *makeFieldFromFile(char *fileName, time_t *time, int *turns);  //
extern Field *makeFieldFromField(const Field *f);                           //
                                                                            //
// Destructors                                                              //
extern void  freeField(Field *f);                                           //
                                                                            //
// Member functions                                                         //
extern void printField(const Field *f);                                     //
extern ObjType peekAhead(const Field *f, Coord c, Dir d, unsigned n);       //
extern ObjType objTypeAtCoord(const Field *f, Coord c);                     //
                                                                            //
// ===========================================================================

// Internal functions ========================================================
                                                                            //
static inline Field *allocateField(UIntPair size);                          //
static inline void parseAndFill(Field *f, char *fileName);                  //
                                                                            //
static inline UIntPair getBoardDims(char *levelName);                       //
static inline bool isValidBoardChar(char c);                                //
                                                                            //
// ===========================================================================

#endif

