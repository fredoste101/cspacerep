#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "notes/note.h"

#define CONFIG_FILE "erispacerep.conf"
#define NOTES_FILE "notes/noteList.bin"
#define MAX_NOTES_TO_STUDY 50

#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR 3600
#define HOURS_IN_DAY 24
#define SECONDS_IN_DAY SECONDS_IN_HOUR * HOURS_IN_DAY

#define MAX_NUM_OF_FLASHCARD_PILES 7


typedef struct programConfiguration
{
    unsigned int nextNoteId;
    unsigned int timeLastStarted;
    unsigned int offsetInDayToStudy;   ///< offset in seconds in day when study period should be (I.E updates)
    unsigned int numOfNotes;
} programConfiguration;


typedef enum
{
  QUIT = 0,
  LIST_NOTES,
  STUDY,
  FLASHCARD_STUDY,
  CREATE_NOTE,
  CHANGE_NOTE,
  REMOVE_NOTE
} MAIN_MENU_CHOICES;


void printMainMenu();

int readProgramConfigFile(programConfiguration* configP);
int saveProgrammingConfigFile(programConfiguration* configP);


void setDefaultConfigValues(programConfiguration* configP);

void createProgramConfigFile(programConfiguration* configP);

void getOffsetInDayToStudy(programConfiguration* configP);

void printConfiguration(programConfiguration* configP);

int createNote(note* noteP, unsigned int noteId);

int saveNotesToFile(noteList* notesP);


int loadNotesFromFile(noteList* notes);

int isNoteIdInList(noteList* noteListP, unsigned int id);



time_t getStudyTime(time_t currentTime, 
                    unsigned int offsetInDayToStudy);


int studyLoop(unsigned int numOfNotesToStudy, note* notesToStudy[MAX_NOTES_TO_STUDY], time_t studyTime);

void studyNote(note* currentNoteP);

char answerNote();

float calculateNewEf(float oldEf, unsigned int q);

void listNotes(noteList* noteListP);


void removeNewlineAtEnd(char* string);

void changeNote(noteList* noteListP);

void study(programConfiguration* configP, noteList* noteListP);

int main()
{
    printf("\n*** erispacerep ***\n");

    programConfiguration config;


    if(!readProgramConfigFile(&config))
    {
        setDefaultConfigValues(&config);
        getOffsetInDayToStudy(&config);
        createProgramConfigFile(&config);
    }

    printConfiguration(&config);

    noteList notes;
    memset(&notes, 0, sizeof(notes));

    if(!loadNotesFromFile(&notes))
    {
        if(!createNotesFile())
        {
            perror("could not create notes file. FATAL");
            exit(1);
        }
    }

    note* notesToStudy[MAX_NOTES_TO_STUDY];
 
    printMainMenu();

    unsigned int choice;

    while(1)
    {
        printf("\nChoice: ");
        scanf("%u", &choice);

        switch(choice)
        {
            case QUIT:
                printf("\nBlip blop goodbye...\n");
                saveNotesToFile(&notes);
                return 0;
            break;
            case LIST_NOTES:
                listNotes(&notes);
            break;
            case STUDY:
                study(&config, &notes);
            break;
            case FLASHCARD_STUDY:
                ;
                

                if(notes.size == 0)
                {
                    printf("No notes to study");
                }
                else
                {
                    noteList flashcardPiles[MAX_NUM_OF_FLASHCARD_PILES];

                    memset(&flashcardPiles, 0, sizeof(noteList) * MAX_NUM_OF_FLASHCARD_PILES);

                    unsigned int i = 0;

                    for(i = 0; i < notes.size; i++)
                    {
                        note tmpNote;

                        memcpy(&tmpNote, &notes.list[i], sizeof(note));

                        addNote(&tmpNote, &flashcardPiles[0]);
                    }

                    while(1)
                    {
                        note* currentNoteP;

                        unsigned int flashcardPileIndex = 0;

                        //Find the first best note...
                        for(i = 0; i < MAX_NUM_OF_FLASHCARD_PILES; i++)
                        {
                            if(flashcardPiles[i].size > 0)
                            {
                                currentNoteP = &flashcardPiles[i].list[0];  //We could randomize 0-size here instead to make it fun
                                flashcardPileIndex = i;
                                break;
                            }
                        }

                        studyNote(currentNoteP);

                        int q = answerNote();

                        if(q == '9')
                        {
                            printf("\nending flashcards...\n");
                            break;
                        }
                        else
                        {
                            switch(q)
                            {
                                case '1':
                                    //Do nothing
                                break;
                                case '2':
                                case '3':
                                case '4':
                                    ;
                                    note tmpNote;

                                    memcpy(&tmpNote, currentNoteP, sizeof(tmpNote));

                                    removeNote(&flashcardPiles[flashcardPileIndex], 0); //Always remove first as of now. since that one is always choosen now.

                                    char tmpShit = q;

                                    int newFlashcardPileIndex = flashcardPileIndex + (atoi(&tmpShit) - 1);

                                    if(newFlashcardPileIndex >= MAX_NUM_OF_FLASHCARD_PILES)
                                    {
                                        newFlashcardPileIndex = MAX_NUM_OF_FLASHCARD_PILES - 1;
                                    }

                                    addNote(&tmpNote, &flashcardPiles[newFlashcardPileIndex]);

                                break;
                                default:
                                    printf("\nUnknown option...\n");
                                break;
                            }
                        }
                    }
                }

            break;
            case CREATE_NOTE: //create note
                printf("\nCreate note\n");

                note newNote;

                if(createNote(&newNote, config.nextNoteId))
                {
                    config.nextNoteId++;
                    config.numOfNotes++;
                    saveProgrammingConfigFile(&config);
                    addNote(&newNote, &notes);

                    saveNotesToFile(&notes);
                }
            break;
            case CHANGE_NOTE:
                changeNote(&notes);
            break;
            case REMOVE_NOTE: //Remove note
                printf("\nNote id:");
                
                scanf("%d", &choice);

                if(0 <= choice && choice < notes.size)
                {
                    removeNote(&notes, choice);
                }
                else
                {
                    printf("Illegal note id given\n");
                }
            break;
            default:
                printf("\nwhat ey?\n");
                printMainMenu();
            break;
        }

    }

    

    return 0;
}


void printMainMenu()
{
    printf("\n");
    printf("***********************\n");
    printf("* 0 quit              *\n");
    printf("* 1 list notes        *\n");
    printf("* 2 study             *\n");
    printf("* 3 flashcard study   *\n");
    printf("* 4 create note       *\n");
    printf("* 5 change note       *\n");
    printf("* 6 remove note       *\n");
    printf("***********************\n");
}


void study(programConfiguration* configP, noteList* noteListP)
{
    note* notesToStudy[MAX_NOTES_TO_STUDY];

    const time_t currentTime = time(NULL);

    time_t studyTime = getStudyTime(currentTime, configP->offsetInDayToStudy);

    unsigned int numOfNotesToStudy = getNotesToStudy(noteListP, 
                                                     studyTime, 
                                                     notesToStudy, 
                                                     MAX_NOTES_TO_STUDY);

    int numOfActiveNotes = 0;

    if(numOfNotesToStudy > 0)
    {
        do
        {
            numOfActiveNotes = studyLoop(numOfNotesToStudy, 
                                         notesToStudy, 
                                         studyTime);

        } while(numOfActiveNotes > 0);
    }
    else
    {
        printf("\nNo notes to study\n");
    }
}


int readProgramConfigFile(programConfiguration* configP)
{
    FILE* configFile = fopen(CONFIG_FILE, "r");

    if(configFile == NULL)
    {
        return 0;
    }
    else
    {
        size_t memberRead = fread((void*)configP, sizeof(*configP), 1, configFile);

        if(memberRead != 1)
        {
            perror("bytesRead is not equal to config size. FATAL.");
            error(1);
        }

        fclose(configFile);
        return 1;
    }
}


int saveProgrammingConfigFile(programConfiguration* configP)
{
    FILE* configFile = fopen(CONFIG_FILE, "w");

    if(configFile == NULL)
    {
        perror("error opening file with w. FATAL");
        error(1);
    }

    fwrite(configP, sizeof(*configP), 1, configFile);

    fclose(configFile);
}

void setDefaultConfigValues(programConfiguration* configP)
{
    configP->nextNoteId         = 0;
    configP->timeLastStarted    = 0;   ///< @todo change this to time values later
    configP->offsetInDayToStudy = 0;  
    configP->numOfNotes         = 0;
}


void createProgramConfigFile(programConfiguration* configP)
{
    FILE* configFile = fopen(CONFIG_FILE, "w+");

    if(configFile == NULL)
    {
        perror("Could not open/create erispacerep.conf. FATAL");
        error(1);
    }

    size_t bytesWritten = fwrite(configP, sizeof(*configP), 1, configFile);

    fclose(configFile);

}


void getOffsetInDayToStudy(programConfiguration* configP)
{
    unsigned int hour;
    unsigned int minute;

    while(1)
    {
        printf("Hour in day to study [0, 23]?\n");
        scanf("%u", &hour);
        if(hour >= 0 && hour < 24)
        {
            break;
        }
        printf("\n[0, 23] pls\n");
    }

    while(1)
    {
        printf("Minute of hour to study [0, 59]?\n");
        scanf("%u", &minute);
        if(minute >= 0 && minute < 60)
        {
            break;
        }
        printf("\n[0, 59] pls\n");
    }

    configP->offsetInDayToStudy = hour * SECONDS_IN_HOUR + minute * SECONDS_IN_MINUTE;
    
}


void printConfiguration(programConfiguration* configP)
{
    printf("\n*** CONFIG ***\n");
    printf("\nnextNoteId: %u", configP->nextNoteId);
    printf("\nlastLoginTime: %u", configP->timeLastStarted);
    printf("\noffsetInDayToStudy: %u", configP->offsetInDayToStudy);

    printf("\n**************\n");
}


int createNote(note* noteP, unsigned int noteId)
{
    int noteType;
    printf("Type of note (0 - TEXT, 1 - BROWSER, 2 - TEXT_BROWSER)\n");
    scanf("%d", &noteType);

    int c;

    noteP->nextStudyTime    = 0;
    noteP->id               = noteId;
    noteP->ef               = 2.5;
    noteP->interval         = 0;
    noteP->n                = 0;

    switch(noteType)
    {
        case TEXT:
            noteP->type = TEXT;
            printf("front:");

            //flush stdin
            while ((c = getchar()) != '\n' && c != EOF);

            fgets(noteP->front, sizeof(noteP->front), stdin);

            printf("back:");
            fgets(noteP->back, sizeof(noteP->back), stdin);

            removeNewlineAtEnd(noteP->front);
            removeNewlineAtEnd(noteP->back);

            return 1;
        break;
        case BROWSER:
            noteP->type = BROWSER;

            printf("front file:");

            //flush stdin
            while ((c = getchar()) != '\n' && c != EOF);

            fgets(noteP->front, sizeof(noteP->front), stdin);

            if(noteP->front)
            {

            }

            printf("back file:");
            fgets(noteP->back, sizeof(noteP->back), stdin);

            removeNewlineAtEnd(noteP->front);
            removeNewlineAtEnd(noteP->back);

            return 1;
        break;
        case TEXT_BROWSER:
            noteP->type = TEXT_BROWSER;

            printf("front:");

            //flush stdin
            while ((c = getchar()) != '\n' && c != EOF);

            fgets(noteP->front, sizeof(noteP->front), stdin);

            printf("back file:");
            fgets(noteP->back, sizeof(noteP->back), stdin);

            removeNewlineAtEnd(noteP->front);
            removeNewlineAtEnd(noteP->back);

            return 1;
        break;
        default:
            printf("\nUNKNOWN TYPE\n");
            return 0;
        break;
    }
}


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

    return 1;
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
        perror("Could not open notesFile for saving. FATAL");
        exit(1);
    }

    fwrite(notesP->list, sizeof(note), notesP->size, notesFile);

    fclose(notesFile);
}


time_t getStudyTime(time_t currentTime, 
                    unsigned int offsetInDayToStudy)
{

    struct tm timeStruct;
    struct tm* tmpP;

    tmpP = localtime(&currentTime);

    memcpy(&timeStruct, tmpP, sizeof(*tmpP));

    time_t studyTime = currentTime - (timeStruct.tm_hour * SECONDS_IN_HOUR + timeStruct.tm_min * SECONDS_IN_MINUTE + timeStruct.tm_sec) + offsetInDayToStudy;

    if(studyTime > currentTime)
    {
        studyTime -= SECONDS_IN_DAY;
    }

    return studyTime;
}



int studyLoop(unsigned int numOfNotesToStudy, 
              note* notesToStudy[MAX_NOTES_TO_STUDY], 
              time_t studyTime)
{
    int numOfActiveNotes = 0;

    unsigned int i = 0;

    for(i = 0; i < numOfNotesToStudy; i++)
    {
        note* currentNoteP = notesToStudy[i];

        if(currentNoteP == NULL)
        {
            continue;
        }

        numOfActiveNotes++;

        studyNote(currentNoteP);

        int q = answerNote();


        switch(q)
        {
            case '0':
                printf("Skipping...\n");
            break;
            case '1':
                currentNoteP->interval = 0;
            break;
            case '2':
            case '3':
            case '4':
                ;
                char tmpShit = q;

                q = atoi(&tmpShit);    //It was a ascii, but now itsa me, integer

                q++; //To fit supermemos rankings (0-5), of which 3,4 and 5 are good

                
                if(currentNoteP->interval == 0)
                {
                    currentNoteP->interval      = 1;
                }
                else if(currentNoteP->interval == 1)
                {
                    currentNoteP->ef            = calculateNewEf(currentNoteP->ef, q);;
                    currentNoteP->interval      = 6;
                }
                else
                {
                    currentNoteP->interval      = currentNoteP->interval * currentNoteP->ef;
                    currentNoteP->ef            = calculateNewEf(currentNoteP->ef, q);;
                }

                currentNoteP->nextStudyTime = studyTime + SECONDS_IN_DAY * currentNoteP->interval - 1; //Add interval num of seconds
                                                                                                    // -1 is to get some time buffer for check
                notesToStudy[i] = NULL; //This one is done
            break;
            case '9':
                return -1;
            break;
            default:
                printf("unknown option %u. skipping...\n", q);    
            break;
        }

    }

    return numOfActiveNotes;
}


void studyNote(note* currentNoteP)
{
    char buffer[1050];

    switch(currentNoteP->type)
    {
        case TEXT:
            printf("\n*********************\n%s", currentNoteP->front);
            printf("\n*********************\n");
            printf("(SPACE) show answer");

            while(fgetc(stdin) != ' ');

            printf("*********************\n%s", currentNoteP->back);
            printf("\n*********************\n");
        break;
        case BROWSER:
            ;
            sprintf(buffer, "firefox %s", currentNoteP->front);
            system(buffer);

            printf("(SPACE) show answer (opens another web page)");
            while(fgetc(stdin) != ' ');

            sprintf(buffer, "firefox %s", currentNoteP->back);
            system(buffer);
        break;
        case TEXT_BROWSER:
            ;
            printf("\n*********************\n%s", currentNoteP->front);
            printf("\n*********************\n");
            printf("(SPACE) show answer (opens web page)");

            while(fgetc(stdin) != ' ');

            
            sprintf(buffer, "firefox %s", currentNoteP->back);
            system(buffer);
        break;
        default:
            printf("oh noes :(\n");
        break;

    }
}


char answerNote()
{
    printf("0 - (skip)  1 - (redo)  2 - (good)  3 - (very good)  4 - (perfect)   9 - quit\n");
                
    int q;
    while ((q = fgetc(stdin)) == '\n');

    return q;
}


float calculateNewEf(float oldEf, unsigned int q)
{
    float newEf = oldEf + (0.1-(5-q)*(0.08+(5-q)*0.02));

    if(newEf < 1.3)
    {
        newEf = 1.3;
    }

    if(newEf > 2.5)
    {
        newEf = 2.5;
    }

    return newEf;
}


void listNotes(noteList* noteListP)
{
    printf("\n*** NOTES ***\n");

    unsigned int i = 0;
    
    printf("\n-------------------------------------------------------------------");
    printf("\n|  ID  |   FRONT   |   BACK   | EF  | INTERVAL |       TIME       |");
    printf("\n-------------------------------------------------------------------");

    for(i = 0; i < noteListP->size; i++)
    {
        note* currentNoteP = &noteListP->list[i];

        const time_t tempT = currentNoteP->nextStudyTime;

        struct tm* ltP = localtime(&tempT);

        struct tm calendarTime;
        memcpy(&calendarTime, ltP, sizeof(struct tm));

        char tmpFront[9]    = {'\0'};
        char tmpBack[8]     = {'\0'};

        unsigned int maxStrlen = 9;

        if(strlen(currentNoteP->front) < 8)
        {
            maxStrlen = strlen(currentNoteP->front);
        }

        strncpy(tmpFront, currentNoteP->front, maxStrlen);

        maxStrlen = 7;

        if(strlen(currentNoteP->back) < 7)
        {
            maxStrlen = strlen(currentNoteP->back);
        }

        strncpy(tmpBack, currentNoteP->back, maxStrlen);


        printf("\n| %4u | %9s | %8s | %0.1f | %8d | %4d-%02d-%02d %02d:%02d |", 
                currentNoteP->id, 
                tmpFront, 
                tmpBack,
                currentNoteP->ef,
                currentNoteP->interval,
                calendarTime.tm_year + 1900,
                calendarTime.tm_mon + 1,
                calendarTime.tm_mday + 1,
                calendarTime.tm_hour,
                calendarTime.tm_min);

        printf("\n-------------------------------------------------------------------");
    }

    printf("\nNotes: %u", noteListP->size);
    printf("\n-------------------------------------------------------------------\n");

}


/**
 * @brief if string ends with \n replaces that with \0
 * 
 * @param[in/out] string 
 */
void removeNewlineAtEnd(char* string)
{
    if(string[strlen(string) - 1] == '\n')
    {
        string[strlen(string) - 1] = '\0';
    }
}


void changeNote(noteList* noteListP)
{
    printf("\nNote id:");

    unsigned int noteId;

    scanf("%d", &noteId);

    noteId = isNoteIdInList(noteListP, noteId);
   
    if(noteId >= 0)
    {
        note* noteP = &noteListP->list[noteId];

        char c;

        //flush stdin
        while ((c = getchar()) != '\n' && c != EOF);

        printf("front: (%s)", noteP->front);
        fgets(noteP->front, sizeof(noteP->front), stdin);

        printf("back: (%s)", noteP->back);
        fgets(noteP->back, sizeof(noteP->back), stdin);

        removeNewlineAtEnd(noteP->front);
        removeNewlineAtEnd(noteP->back);

    }
    else
    {
        printf("Illegal note id: %u given\n", noteId);
    }
}


int isNoteIdInList(noteList* noteListP, unsigned int id)
{
    unsigned int i;
    
    for(i = 0; i < noteListP->size; i++)
    {
        if(noteListP->list[i].id == id)
        {
            return i;
        }
    }

    return -1;
}