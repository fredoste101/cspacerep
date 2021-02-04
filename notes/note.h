#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INCREMENT_NOTE_LIST_SIZE 30

typedef enum noteType
{
    TEXT,
    BROWSER,
    TEXT_BROWSER    //Text as front, browser as back
} noteType;


typedef struct note
{
    unsigned int id;
    noteType type;
    time_t nextStudyTime;    ///< next time to study in seconds since epoch
    unsigned int interval;          ///< number of days to wait until next study time. Used in SM-2 algo
    float ef;   //Easinessfactor as taken from: https://www.supermemo.com/en/archives1990-2015/english/ol/sm2
    unsigned int n;
    char front[1024];                 ///< either text to be displayed in terminal, or could be file to be viewed in e.g firefox
    char back[1024];                   ///< same as front, but the answer then :)

} note;



typedef struct noteList
{
    unsigned int size;
    unsigned int maxSize;
    note* list;
} noteList;


void addNote(note* noteP, noteList* noteListP)
{
    if(noteListP->size == noteListP->maxSize)
    {
        note* temp = noteListP->list;

        noteListP->maxSize += INCREMENT_NOTE_LIST_SIZE;

        noteListP->list = (note*)malloc(sizeof(note) * noteListP->maxSize);

        memcpy(noteListP->list, temp, noteListP->size * sizeof(note));

        free(temp);

    }

    memcpy(&noteListP->list[noteListP->size], noteP, sizeof(note));

    noteListP->size++;
}


void removeNote(noteList* noteListP, unsigned int index)
{
    if(index < noteListP->size)
    {
        memcpy(&noteListP->list[index], 
               &noteListP->list[index+1], 
               sizeof(note) * (noteListP->size - (index + 1)));

        noteListP->size--;
    }
}


unsigned int getNotesToStudy(noteList* noteListP, 
                             const time_t prevStudyTime, 
                             note** notesToStudyP, 
                             const unsigned int maxNumOfStudyNotes)
{
    unsigned int i = 0;
    unsigned int noteToStudyIndex = 0;

    for(i = 0; i < noteListP->size; i++)
    {
        if(((noteListP->list[i].nextStudyTime == 0) || (noteListP->list[i].nextStudyTime < prevStudyTime)) && 
           noteToStudyIndex < maxNumOfStudyNotes)
        {
            notesToStudyP[noteToStudyIndex] = &noteListP->list[i];
            noteToStudyIndex++;
        }
    }

    return noteToStudyIndex;
}