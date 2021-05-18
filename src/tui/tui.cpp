#include "tui.h"


WINDOW* TUI::getBox(unsigned int posY, 
                    unsigned int posX, 
                    unsigned int height, 
                    unsigned int width)
{
    WINDOW* winP = newwin(height, 
                          width, 
                          posY, 
                          posX);

    simpleBox(winP);

    return winP;
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
void TUI::simpleBox(WINDOW* winP)
{
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


TUI::TUI()
{
    /* Initialzie window */
    initscr();

    /* Take in all characters written by user */
    //raw();

    /* Do not echo user input */
    noecho();

    base = WindowManager(stdscr);

    int maxY;
    int maxX;

    getmaxyx(stdscr, maxY, maxX);

    WINDOW* headerWinP  = newwin(HEADER_HEIGHT, maxX, 0, 0);
    WINDOW* contentWinP = newwin(maxY - HEADER_HEIGHT, maxX, HEADER_HEIGHT, 0);
    
    WindowManager* headerP  = new WindowManager(headerWinP);
    WindowManager* contentP = new WindowManager(contentWinP);

    base.appendChild(headerP);
    base.appendChild(contentP);
    
}


void TUI::showStartScreen()
{
    showHeader();
    showMenu();

    base.updateWindows();
}


void TUI::showConfigurationScreen(programConfiguration* configP)
{

    int winHeight   = 20;
    int winWidth    = 50;
    int winYPos     = (base.height() - winHeight) / 2;
    int winXPos     = (base.width() - winWidth) / 2;

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


void TUI::listDecks(DeckContainer* deckContainerP)
{
    WindowManager* contentP = base.getChildAtIndex(1);  //Index 1 is the content

    contentP->clearChildren();

    const unsigned int deckListElementHeight = 3;
    const unsigned int deckListelementWidth = 20;

    int startY, startX;

    getbegyx(contentP->getBase(), startY, startX);

    startY += 2;
    startX += 3;

    for(unsigned int i = 0; i < deckContainerP->numOfDecks(); i++)
    {
        deck* currentDeckP = deckContainerP->getDeckByIndex(i);

        WINDOW* winP = getBox(startY, 
                              startX, 
                              deckListElementHeight, 
                              deckListelementWidth);

        wrefresh(winP);

        printCenter(winP, currentDeckP->name->c_str());

        WindowManager* winManP = new WindowManager(winP);

        contentP->appendChild(winManP);
    }

    contentP->updateWindows();

    char ch = getch();
}


void TUI::createDeckForm(DeckContainer* deckCoontainerP)
{
     WindowManager* contentP = base.getChildAtIndex(1);

     contentP->clearChildren();

     //TODO: form to give:
     //name
}


void TUI::showHeader()
{
    WindowManager* headerP = base.getChildAtIndex(0);  //Index 0 is the header

    WINDOW* headerWinP = headerP->getBase();

    simpleBox(headerWinP);
    printCenter(headerWinP, "*** CSPACEREP ***");
}


void TUI::showMenu()
{
    WindowManager* contentP = base.getChildAtIndex(1);  //Index 1 is the content

    contentP->clearChildren();


    WINDOW* contentWindowP = contentP->getBase();

    simpleBox(contentWindowP);

    wrefresh(contentWindowP);

    int menuStartX, menuStartY;

    getbegyx(contentWindowP, menuStartY, menuStartX);

    const char* menuStringList[3] = 
    {
        "0 QUIT", 
        "1 LIST DECKS",
        "2 CREATE DECK"
    };

    WINDOW* menuWindow;

    unsigned int i;

    for(i = 0; i < 3; i++)
    {
        int startY = (menuStartY+2);
        int startX = (menuStartX+3) + i * 21;

        menuWindow = getBox(startY, startX, 3, 20);
        WindowManager* menuWindowManagerP = new WindowManager(menuWindow);

        printCenter(menuWindow, menuStringList[i]);

        contentP->appendChild(menuWindowManagerP);
    }

    contentP->updateWindows();
}
