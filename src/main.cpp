#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <ncurses.h>
#include <unistd.h>


#include "note/note.h"
#include "timeUtils.h"
#include "deck/deckContainer.h"
#include "note/noteContainer.h"
#include "config/config.h"

#include "tui/tui.h"

#include "storage/configStorage.h"

#include "study/memAlgo.h"


#define VERSION 0

#define MAX_NUM_OF_FLASHCARD_PILES 7


typedef enum
{
  QUIT = 0,
  LIST_DECKS,
  CREATE_DECK,
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

int createNewNote(note* noteP, unsigned int noteId);

time_t getStudyTime(time_t currentTime, 
                    unsigned int offsetInDayToStudy);

void removeNewlineAtEnd(char* string);


int initConfiguration(CSPACEREP::Config* configP, TUI* tuiP);


int main()
{
    TUI tui = TUI();

    DeckContainer deckContainer = DeckContainer();

    std::string deckFile = DECK_FILE;

    deckContainer.setFile(deckFile);

    if(!deckContainer.load())
    {
        exit(1);
    }

    NoteContainer noteContainer = NoteContainer(&deckContainer);

    noteContainer.setFileName(NOTES_FILE);
    
    noteContainer.load();

    tui.setDeckContainer(&deckContainer);
    tui.setNoteContainer(&noteContainer);

    CSPACEREP::Config myConfig;

    //initConfiguration(&myConfig, &tui);


    //MAIN LOOP

    while(1)
    {
        
        tui.startPage();

        char choice = getch();

        switch(choice - 48)
        {
            case QUIT:
            {
                deckContainer.save();
                noteContainer.save();
                return 0;
            }
            break;

            case LIST_DECKS:
            {
                tui.deckListPage(&deckContainer);
            }
            break;

            case CREATE_DECK:
            {
                tui.createDeckPage(&deckContainer);
            }
            break;


            case LIST_NOTES:
            break;

            case STUDY:
            break;

            case FLASHCARD_STUDY:
            break;

            case CREATE_NOTE: break; 
            break;

            case CHANGE_NOTE: break;
            break;

            case DELETE_NOTE: break;
            break;

            default:
                //printf("\nwhat ey?\n");
            break;
        }
    }

    return 0;
}



void setDefaultConfigValues(programConfiguration* configP)
{
    configP->metaData.sizeOfConfigInBytes   = sizeof(*configP);
    configP->metaData.version               = VERSION;
    configP->nextNoteId                     = 0;
    configP->timeLastStarted                = 0;   ///< @todo change this to time values later
    configP->offsetInDayToStudy             = 0;  
    configP->numOfNotes                     = 0;

    configP->noteSizeInBytes                = sizeof(note);
    configP->deckSizeInBytes                = sizeof(deck);
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


int initConfiguration(CSPACEREP::Config* configP, TUI* tuiP)
{
    configP->setFileName(CONFIG_FILE);

    /*
    if(!readProgramConfigFile(configP))
    {
        setDefaultConfigValues(configP);
        tuiP->getOffsetInDayToStudy(configP);
        createProgramConfigFile(configP);
    }

    tuiP->showConfigurationScreen(configP);
    */
}