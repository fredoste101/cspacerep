#ifndef STUDY_INCLUSION_PROTECTION
#define STUDY_INCLUSION_PROTECTION

#include <ncurses.h>
#include "memAlgo.h"
#include "../notes/note.h"
#include "../timeUtils.h"
#include "../tui/tuiConstants.h"
#include "../tui/tui.h"

#define MAX_NOTES_TO_STUDY 50

int studyNote(note* currentNoteP);

int studyLoop(unsigned int numOfNotesToStudy, 
              note* notesToStudy[MAX_NOTES_TO_STUDY], 
              time_t studyTime);

void startStudy(time_t studyTime, 
           noteList* noteListP);


#endif
