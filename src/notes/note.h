#ifndef NOTE_H_INCLUSION_PROTECTION
#define NOTE_H_INCLUSION_PROTECTION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../deck/deck.h"

#define INCREMENT_NOTE_LIST_SIZE 30

typedef enum noteType
{
    TEXT,
    BROWSER,
    TEXT_BROWSER    //Text as front, browser as back
} noteType;

typedef struct noteData
{
    unsigned int    size;  ///< number of characters in data
    char*           data;  ///< either text to be displayed in terminal, or could be file to be viewed in e.g firefox
}noteData;


typedef struct note
{
    unsigned int    id;
    deck*           deckP;
    noteType        type;
    time_t          nextStudyTime;    ///< next time to study in seconds since epoch
    unsigned int    interval;          ///< number of days to wait until next study time. Used in SM-2 algo
    float           ef;   //Easinessfactor as taken from: https://www.supermemo.com/en/archives1990-2015/english/ol/sm2
    unsigned int    n;
    noteData        front;
    noteData        back;

} note;



typedef struct noteList
{
    unsigned int    size;
    unsigned int    maxSize;
    note*           list;
} noteList;


void addNote(note* noteP, noteList* noteListP);
void removeNote(noteList* noteListP, unsigned int index);
unsigned int getNotesToStudy(noteList* noteListP, 
                             const time_t prevStudyTime, 
                             note** notesToStudyP, 
                             const unsigned int maxNumOfStudyNotes);

#endif