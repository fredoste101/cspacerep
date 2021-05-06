#include "note.h"



void addNote(note* noteP, noteList* noteListP)
{
    if(noteListP->size == noteListP->maxSize)
    {
        note* temp = noteListP->list;

        noteListP->maxSize += INCREMENT_NOTE_LIST_SIZE;

        noteListP->list = (note*)malloc(sizeof(note) * noteListP->maxSize);

        memcpy(noteListP->list, 
               temp, 
               (noteListP->size * sizeof(note)));

        free(temp);

    }

    memcpy(&noteListP->list[noteListP->size], 
           noteP, 
           sizeof(*noteP));

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