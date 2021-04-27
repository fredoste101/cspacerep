#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "storageConstants.h"
#include "../notes/note.h"

int saveNotesToFile(noteList* notesP);
int createNotesFile();

int loadNotesFromFile(noteList* notes);
