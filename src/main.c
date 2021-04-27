#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "notes/note.h"
#include "timeUtils.h"
#include "study/study.h"
#include "config/config.h"

#include "tui/tuiUtils.h"

#include "storage/configStorage.h"
#include "storage/notesStorage.h"

#include "study/memAlgo.h"


#define VERSION 0

#define MAX_NUM_OF_FLASHCARD_PILES 7


typedef enum
{
  QUIT = 0,
  LIST_NOTES,
  STUDY,
  FLASHCARD_STUDY,
  CREATE_NOTE,
  CHANGE_NOTE,
  DELETE_NOTE
} MAIN_MENU_CHOICES;


void fatalError(const char* msg)
{
    endwin();
    perror(msg);
    exit(1);
}


void setDefaultConfigValues(programConfiguration* configP);

void getOffsetInDayToStudy(programConfiguration* configP, int maxY, int maxX);

void printConfiguration(programConfiguration* configP, int maxY, int maxX);

int createNewNote(note* noteP, unsigned int noteId);

void flashcardStudy(noteList* notesP);

int isNoteIdInList(noteList* noteListP, unsigned int id);


void initNotes(noteList* notesP);


time_t getStudyTime(time_t currentTime, 
                    unsigned int offsetInDayToStudy);


void listNotes(noteList* noteListP);


void removeNewlineAtEnd(char* string);

void changeNote(noteList* noteListP);

int initConfiguration(programConfiguration* configP, int maxY, int maxX);


void quit(noteList* notesP, 
          WINDOW*   headerWinP, 
          WINDOW*   menuWinP);


void study(programConfiguration* configP,
                noteList* notesP);


void createNote(programConfiguration* configP,
                noteList* notesP);


void deleteNote(programConfiguration* configP,
                noteList* notesP);


int main()
{
    /* Initialzie window */
    initscr();

    /* Take in all characters written by user */
    //raw();

    /* Do not echo user input */
    noecho();

    int maxY;
    int maxX;

    getmaxyx(stdscr, maxY, maxX);

    WINDOW* headerWinP  = newwin(HEADER_HEIGHT, maxX, 0, 0);
    WINDOW* menuWinP    = newwin(maxY - HEADER_HEIGHT, maxX, HEADER_HEIGHT, 0);

    refresh();

    initHeaderWindow(headerWinP, maxX);
    
    refresh();
    wrefresh(headerWinP);

    programConfiguration config;

    initConfiguration(&config, maxY, maxX);

    noteList notes;

    memset(&notes, 0, sizeof(notes));

    initNotes(&notes);


    //MAIN LOOP

    while(1)
    {
        initMenuWindow(menuWinP);

        char choice = getch();

        switch(choice - 48)
        {
            case QUIT:
                quit(&notes, headerWinP, menuWinP);
                return 0;
            break;

            case LIST_NOTES:
                listNotes(&notes);
            break;

            case STUDY:
                study(&config, &notes);
            break;

            case FLASHCARD_STUDY:
                flashcardStudy(&notes);
            break;

            case CREATE_NOTE: break; //create note
                createNote(&config, &notes);
            break;

            case CHANGE_NOTE: break;
                changeNote(&notes);
            break;

            case DELETE_NOTE: break;//Remove note
                deleteNote(&config, &notes);
            break;

            default:
                //printf("\nwhat ey?\n");
            break;
        }
    }

    return 0;
}


void quit(noteList* notesP, 
          WINDOW*   headerWinP, 
          WINDOW*   menuWinP)
{
    if(0 > saveNotesToFile(notesP))
    {
	fatalError("Could not save notes to file");
    }

    delwin(headerWinP);
    delwin(menuWinP);
    refresh();
    endwin();
}


void listNotes(noteList* noteListP)
{
    int x, y;
    getmaxyx(stdscr, y, x);

    int winHeight = y - HEADER_HEIGHT;
    int winWidth = x;
    int winYPos = HEADER_HEIGHT;
    int winXPos = 0;

    WINDOW* noteListWinP = newwin(winHeight, winWidth, winYPos, winXPos);
    refresh();

    simpleBox(noteListWinP);
    wrefresh(noteListWinP);

    wmove(noteListWinP, 1, 1);
    wprintw(noteListWinP, "  ID  |   FRONT   |   BACK   | EF  | INTERVAL |       TIME       |");
    wmove(noteListWinP, getcury(noteListWinP) + 1, 1);
    wprintw(noteListWinP, "------------------------------------------------------------------");

    
    unsigned int i;

    for(i = 0; i < noteListP->size; i++)
    {
        note* currentNoteP = &noteListP->list[i];

        const time_t tempT = currentNoteP->nextStudyTime;

        struct tm* ltP = localtime(&tempT);

        struct tm calendarTime;
        memcpy(&calendarTime, ltP, sizeof(struct tm));

        char tmpFront[9]    = {'\0'};
        char tmpBack[9]     = {'\0'};

        unsigned int maxStrlen = 8;

        if(strlen(currentNoteP->front) < 8)
        {
            maxStrlen = strlen(currentNoteP->front) - 1;
        }

        strncpy(tmpFront, currentNoteP->front, maxStrlen);

        maxStrlen = 7;

        if(strlen(currentNoteP->back) < 7)
        {
            maxStrlen = strlen(currentNoteP->back);
        }

        strncpy(tmpBack, currentNoteP->back, maxStrlen);

        
        mvwprintw(noteListWinP, getcury(noteListWinP) + 1, 1, " %4u | %9s | %8s | %0.1f | %8d | %4d-%02d-%02d %02d:%02d |", 
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

        mvwprintw(noteListWinP, getcury(noteListWinP) + 1, 1, "------------------------------------------------------------------");
    }

    mvwprintw(noteListWinP, getcury(noteListWinP) + 1, 1, "Notes: %u", noteListP->size);
    mvwprintw(noteListWinP, getcury(noteListWinP) + 1, 1, "------------------------------------------------------------------");


    wrefresh(noteListWinP);
    getch();

    werase(noteListWinP);
    wrefresh(noteListWinP);
    delwin(noteListWinP);

}


void study(programConfiguration* configP,
                noteList* notesP)
{
    const time_t currentTime = time(NULL);
    time_t studyTime = getStudyTime(currentTime, 
                                    configP->offsetInDayToStudy);

    startStudy(studyTime, notesP);
}


void showNoNotesToStudyWindow(WINDOW* noNotesToStudyWinP)
{
    refresh();
    simpleBox(noNotesToStudyWinP);
    wrefresh(noNotesToStudyWinP);
    wmove(noNotesToStudyWinP, 1, 1);
    wprintw(noNotesToStudyWinP, "No notes to study");
    getch();
    werase(noNotesToStudyWinP);
    delwin(noNotesToStudyWinP);
}


void removeNoNotesToStudyWindow(WINDOW* noNotesToStudyWinP)
{
    werase(noNotesToStudyWinP);
    delwin(noNotesToStudyWinP);
}


void flashcardStudy(noteList* notesP)
{
    if(notesP->size == 0)
    {
        WINDOW* noNotesToStudyWinP = newwin(5, 20, 5, 5);
        showNoNotesToStudyWindow(noNotesToStudyWinP);
        getch();
        removeNoNotesToStudyWindow(noNotesToStudyWinP);
    }
    else
    {
        noteList flashcardPiles[MAX_NUM_OF_FLASHCARD_PILES];

        memset(&flashcardPiles, 0, sizeof(noteList) * MAX_NUM_OF_FLASHCARD_PILES);

        unsigned int i = 0;

        for(i = 0; i < notesP->size; i++)
        {
            note tmpNote;

            memcpy(&tmpNote, &notesP->list[i], sizeof(note));

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

            int q = studyNote(currentNoteP);


            if(q == 9)
            {
                break;
            }
            else
            {
                switch(q)
                {
                    case 1:
                        //Do nothing
                    break;
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                        ;
                        note tmpNote;

                        memcpy(&tmpNote, currentNoteP, sizeof(tmpNote));

                        removeNote(&flashcardPiles[flashcardPileIndex], 0); //Always remove first as of now. since that one is always choosen now.

                        int newFlashcardPileIndex = flashcardPileIndex + (q - 1);

                        if(newFlashcardPileIndex >= MAX_NUM_OF_FLASHCARD_PILES)
                        {
                            newFlashcardPileIndex = MAX_NUM_OF_FLASHCARD_PILES - 1;
                        }

                        addNote(&tmpNote, &flashcardPiles[newFlashcardPileIndex]);

                    break;
                    default:
                        //printf("\nUnknown option...\n");
                    break;
                }
            }
        }
    }
}


void createNote(programConfiguration* configP,
                noteList* notesP)
{

    note newNote;

    if(createNewNote(&newNote, configP->nextNoteId))
    {
        configP->nextNoteId++;
        configP->numOfNotes++;
        saveProgrammingConfigFile(configP);
        addNote(&newNote, notesP);

        if(0 > saveNotesToFile(notesP))
	{
		fatalError("Could not save notes to file");
	}
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


void deleteNote(programConfiguration* configP,
                noteList* notesP)
{
    return;

    int temp = 0;

    if(0 <= temp && temp < notesP->size)
    {
        removeNote(notesP, temp);
    }
    else
    {
        //Could note remove note
    }
}




void setDefaultConfigValues(programConfiguration* configP)
{
    configP->nextNoteId         = 0;
    configP->timeLastStarted    = 0;   ///< @todo change this to time values later
    configP->offsetInDayToStudy = 0;  
    configP->numOfNotes         = 0;
}


void getOffsetInDayToStudy(programConfiguration* configP, int maxY, int maxX)
{
    int inputTime;
    int hour;
    int minute;

    unsigned int configInputHeight = 5;
    unsigned int configInputWidth = 50;

    unsigned int configInputYPosition = (maxY-configInputHeight)/2;
    unsigned int configInputXPosition = (maxX-configInputWidth)/2;

    WINDOW* configInputWinP = newwin(configInputHeight, 
                                     configInputWidth, 
                                     configInputYPosition, 
                                     configInputXPosition);
    refresh();

    simpleBox(configInputWinP);

    wrefresh(configInputWinP);

    wmove(configInputWinP, 2, 3);

    wprintw(configInputWinP, "Hour in day to study [0, 23]? ");

    wrefresh(configInputWinP);

    int hasGottenInput = 0;

    while(!hasGottenInput)
    {
        char firstInput;

        while(1)
        {
            firstInput = getch();

            if(firstInput >= '0' && firstInput <= '9')
            {
                waddch(configInputWinP, firstInput);
                wrefresh(configInputWinP);
                break;
            }
        }

        while(1)
        {
            char secondInput = getch();

            if(secondInput == 10)
            {
                inputTime =  firstInput - 48;
                hasGottenInput = 1;
                break;
            }
            else
            {
                if(firstInput == '1' && 
                    secondInput >= '0' && secondInput <= '9' ||
                    firstInput == '2' && 
                    secondInput >= '0' && secondInput <= '3')
                {
                    inputTime = (firstInput - 48) * 10 + secondInput - 48;
                    hasGottenInput = 1;
                    break;
                }
            }
        }
    }

    hour = inputTime;

    werase(configInputWinP);
    wrefresh(configInputWinP);
    
    simpleBox(configInputWinP);

    wrefresh(configInputWinP);
    wmove(configInputWinP, 2, 3);
    wprintw(configInputWinP, "Minute of hour to study [0, 59]? ");
    wrefresh(configInputWinP);


    hasGottenInput = 0;

    while(!hasGottenInput)
    {
        char firstInput;

        while(1)
        {
            firstInput = getch();

            if(firstInput >= '0' && firstInput <= '9')
            {
                waddch(configInputWinP, firstInput);
                wrefresh(configInputWinP);
                break;
            }
        }

        while(1)
        {
            char secondInput = getch();

            if(secondInput == 10)
            {
                inputTime = firstInput - 48;
                hasGottenInput = 1;
                break;
            }
            else
            {
                if(firstInput > '0' && firstInput <= '5' && 
                   secondInput >= '0' && secondInput <= '9')
                {
                    inputTime = (firstInput - 48) * 10  + secondInput - 48;
                    hasGottenInput = 1;
                    break;
                }
            }
        }
    }

    minute = inputTime;

    werase(configInputWinP);
    wrefresh(configInputWinP);
    delwin(configInputWinP);
    refresh();

    configP->offsetInDayToStudy = hour * SECONDS_IN_HOUR + minute * SECONDS_IN_MINUTE;
    
}


void printConfiguration(programConfiguration* configP, int maxY, int maxX)
{
    int winHeight = 20;
    int winWidth = 50;
    int winYPos = (maxY-winHeight) / 2;
    int winXPos = (maxX-winWidth)/2;

    WINDOW* configWinP = newwin(winHeight, winWidth, winYPos, winXPos);
    refresh();

    simpleBox(configWinP);

    wmove(configWinP, 1, (50 - strlen("*** CONFIG ***"))/2);
    wprintw(configWinP, "*** CONFIG ***");
    wmove(configWinP, 2, 3);
    wprintw(configWinP, "nextNoteId: %u", configP->nextNoteId);
    wmove(configWinP, 3, 3);
    wprintw(configWinP, "lastLoginTime: %u", configP->timeLastStarted);
    wmove(configWinP, 4, 3);
    wprintw(configWinP, "offsetInDayToStudy: %u", configP->offsetInDayToStudy);
    wmove(configWinP, 6, 3);
    wprintw(configWinP, "PRESS ANY KEY TO CONFIRM");

    wrefresh(configWinP);

    getch();
    werase(configWinP);
    wrefresh(configWinP);
    delwin(configWinP);
    refresh();
}


int createNewNote(note* noteP, unsigned int noteId)
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



time_t getStudyTime(time_t currentTime, 
                    unsigned int offsetInDayToStudy)
{

    struct tm timeStruct;
    struct tm* tmpP;

    tmpP = localtime(&currentTime);

    memcpy(&timeStruct, tmpP, sizeof(*tmpP));

    time_t studyTime = 
        currentTime - 
        (timeStruct.tm_hour * SECONDS_IN_HOUR + timeStruct.tm_min * SECONDS_IN_MINUTE + timeStruct.tm_sec) + 
        offsetInDayToStudy;

    if(studyTime > currentTime)
    {
        studyTime -= SECONDS_IN_DAY;
    }

    return studyTime;
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



int initConfiguration(programConfiguration* configP, int maxY, int maxX)
{
    if(!readProgramConfigFile(configP))
    {
        setDefaultConfigValues(configP);
        getOffsetInDayToStudy(configP, maxY, maxX);
        createProgramConfigFile(configP);
    }

    printConfiguration(configP, maxY, maxX);
}


void initNotes(noteList* notesP)
{
    if(0 > loadNotesFromFile(notesP))
    {
        if(!createNotesFile())
        {
            fatalError("could not create notes file. FATAL");
        }
    }
}
