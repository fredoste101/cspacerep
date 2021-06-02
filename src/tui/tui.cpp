#include <stdlib.h>


#include "tui.h"

#include "pages/start/startPage.h"


#include "pages/deck/deckListPage.h"
#include "pages/deck/deckPage.h"
#include "pages/deck/createDeckPage.h"

#include "pages/note/noteListPage.h"

TUI::TUI()
{
    /* Initialzie window */
    initscr();

    /* Take in all characters written by user */
    raw();

    /* Enable keys (<- ^v ->)*/
    keypad(stdscr, true);

    /* Do not echo user input */
    noecho();

    /* Hide cursor */
    curs_set(0);

    if(has_colors())
    {
        start_color();
        use_default_colors();
    }
    

    base = WindowManager(stdscr);

    int maxY;
    int maxX;

    getmaxyx(stdscr, maxY, maxX);
    
    WindowManager* headerP  = getBox(0, 0, HEADER_HEIGHT, maxX);
    WindowManager* contentP = getBox(HEADER_HEIGHT, 0, maxY - HEADER_HEIGHT, maxX); 

    base.appendChild(headerP);
    base.appendChild(contentP);
    
}


WindowManager* TUI::getBox(unsigned int posY, 
                    unsigned int posX, 
                    unsigned int height, 
                    unsigned int width)
{
    WINDOW* winP = newwin(height, 
                          width, 
                          posY, 
                          posX);

    WindowManager* tmpWinManP = new WindowManager(winP);

    simpleBox(tmpWinManP);

    return tmpWinManP;
}


void TUI::printCenter(WINDOW* winP, const char* str)
{
    unsigned int maxY, maxX;

    getmaxyx(winP, maxY, maxX);

    wmove(winP, maxY/2, (maxX - strlen(str))/2);

    wprintw(winP, str);
}


/**
 * @brief create a simple border around the window
 * 
 * @param winP 
 */
void TUI::simpleBox(WindowManager* winManP)
{
    WINDOW* winP = winManP->getBase();

    unsigned int i = 0;
    unsigned int j = 0;

    int height;
    int width;

    getmaxyx(winP, height, width);

    wmove(winP, 0, 0);

    for(j = 0; j < width; j++)
    {
        waddch(winP, '-');
    }

    wmove(winP, (height - 1), 0);

    for(j = 0; j < width; j++)
    {
        waddch(winP, '-');
    }

    wmove(winP, 0, 0);

    for(j = 0; j < height; j++)
    {
        waddch(winP, '|');
        wmove(winP, j, 0);
    }

    wmove(winP, 0, (width - 1));

    for(j = 0; j < height; j++)
    {
        waddch(winP, '|');
        wmove(winP, j, (width - 1));
    }

    // Corners
    
    wmove(winP, 0, 0);
    waddch(winP, '+');

    wmove(winP, 0, (width - 1));
    waddch(winP, '+');

    wmove(winP, (height - 1), 0);
    waddch(winP, '+');

    wmove(winP, (height - 1), (width - 1));
    waddch(winP, '+');
}


void TUI::showConfigurationScreen(programConfiguration* configP)
{

    int winHeight   = 20;
    int winWidth    = 50;
    int winYPos     = (base.height() - winHeight) / 2;
    int winXPos     = (base.width() - winWidth) / 2;

    WINDOW* configWinP = newwin(winHeight, winWidth, winYPos, winXPos);

    refresh();

    WindowManager tmpWinManP = WindowManager(configWinP);

    simpleBox(&tmpWinManP);

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


/**
 * @brief Displays input form and gets user input to fill basic configuration
 * 
 * @param configP 
 */
void TUI::getOffsetInDayToStudy(programConfiguration* configP)
{
    int inputTime;
    int hour;
    int minute;

    unsigned int configInputHeight = 5;
    unsigned int configInputWidth = 50;

    unsigned int configInputYPosition = (base.height() - configInputHeight) / 2;
    unsigned int configInputXPosition = (base.width() - configInputWidth) / 2;

    WINDOW* configInputWinP = newwin(configInputHeight, 
                                     configInputWidth, 
                                     configInputYPosition, 
                                     configInputXPosition);
    refresh();


    WindowManager tmpWinManP = WindowManager(configInputWinP);

    simpleBox(&tmpWinManP);

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
    

    simpleBox(&tmpWinManP);

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


WindowManager* TUI::getContent()
{
    return base.getChildAtIndex(1);
}

WindowManager* TUI::getHeader()
{
    return base.getChildAtIndex(0);
}


void TUI::printMenuHeader(std::string header)
{
    WindowManager* headerWinManP = getHeader();

    //Clear current header
    char buf[headerWinManP->width() - 2];

    memset(buf, ' ', sizeof(char) * headerWinManP->width() - 2);

    wmove(headerWinManP->getBase(), headerWinManP->height() / 2, 1);

    wprintw(headerWinManP->getBase(), buf);

    //Print new header
    headerWinManP->printCenter("CSPACEREP" + header);

    headerWinManP->updateWindows();
}


void TUI::setDeckContainer(DeckContainer* dcP)
{
    deckContainerP = dcP;
}


void TUI::setNoteContainer(NoteContainer* ncP)
{
    noteContainerP = ncP;
}