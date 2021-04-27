
#include "notesStorage.h"


int loadNotesFromFile(noteList* notesP)
{
    FILE* notesFile = fopen(NOTES_FILE, "r");

    if(notesFile == NULL)
    {
        return 0;
    }

    note tempNote;

    while(1)
    {
        size_t membersRead = fread(&tempNote, sizeof(tempNote), 1, notesFile);

        if(membersRead == 1)
        {
            addNote(&tempNote, notesP);
        }
        else
        {
            break;
        }
    }

    fclose(notesFile);

    return 0;
}


int createNotesFile()
{
    FILE* notesFile = fopen(NOTES_FILE, "w+");

    int retValue = (notesFile != NULL);

    if(retValue)
    {
        fclose(notesFile);
    }

    return retValue;
}


int saveNotesToFile(noteList* notesP)
{
    FILE* notesFile = fopen(NOTES_FILE, "w+");

    if(notesFile == NULL)
    {
	    return -1;
    }


    fwrite(notesP->list, sizeof(note), notesP->size, notesFile);

    fclose(notesFile);

    return 0;
}
