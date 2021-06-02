#ifndef NOTE_H_INCLUSION_PROTECTION
#define NOTE_H_INCLUSION_PROTECTION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../deck/deck.h"

typedef enum noteType
{
    TEXT,
    BROWSER,        ///< browser as front, browser as back
    TEXT_BROWSER    ///< Text as front, browser as back
} noteType;


typedef struct note
{
    unsigned int    size;           ///< sizeof(note) to be able to differentiate versions
    unsigned int    id;             ///< unique identifier for this note
    deck*           deckP;          ///< deck this note belongs to
    noteType        type;           ///< determines how the front and back should be dispayed
    time_t          nextStudyTime;  ///< next time to study in seconds since epoch
    unsigned int    interval;       ///< number of days to wait until next study time. Used in SM-2 algo
    float           ef;             ///< Easinessfactor as taken from: https://www.supermemo.com/en/archives1990-2015/english/ol/sm2
    unsigned int    n;
    std::string*    front;
    std::string*    back;
    unsigned int    reserved;       ///< for future use

} note;


#endif