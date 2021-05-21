#include "tui.h"
#include <stdlib.h>

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

    WINDOW* headerWinP  = newwin(HEADER_HEIGHT, maxX, 0, 0);
    WINDOW* contentWinP = newwin(maxY - HEADER_HEIGHT, maxX, HEADER_HEIGHT, 0);
    
    WindowManager* headerP  = new WindowManager(headerWinP);
    WindowManager* contentP = new WindowManager(contentWinP);

    base.appendChild(headerP);
    base.appendChild(contentP);
    
}


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


void TUI::deckListPage(DeckContainer* deckContainerP)
{
    WindowManager* contentP = base.getChildAtIndex(1);  //Index 1 is the content

    contentP->clearChildren();

    int startY, startX;

    startY = contentP->y();
    startX = contentP->x();


    int listHeight = contentP->height() - 2;
    int listWidth = 40;

    WindowManager* deckListWinP = new WindowManager(contentP->y() + 1, 
                                                    contentP->x() + 1,
                                                    listHeight, 
                                                    listWidth);

    simpleBox(deckListWinP->getBase());

    contentP->appendChild(deckListWinP);

    deckListWinP->updateWindows();

    unsigned int numOfDecksInList = listHeight - 2 / 3;

    if(numOfDecksInList > deckContainerP->numOfDecks())
    {
        numOfDecksInList = deckContainerP->numOfDecks();
    }

    const unsigned int deckListElementHeight    = 3;
    const unsigned int deckListelementWidth     = listWidth - 2;

    startY = deckListWinP->y() + 1;
    startX = deckListWinP->x() + 1;

    bool firstElement = true;

    for(unsigned int i = 0; i < numOfDecksInList; i++)
    {
        deck* currentDeckP = deckContainerP->getDeckByIndex(i);

        WINDOW* winP = getBox(startY, 
                              startX, 
                              deckListElementHeight, 
                              deckListelementWidth);


        wrefresh(winP);

        if(firstElement)
        {
            wattron(winP, A_BOLD);
        }

        printCenter(winP, currentDeckP->name->c_str());

        if(firstElement)
        {
            wattroff(winP, A_BOLD);
        }

        firstElement = false;
        
        WindowManager* winManP = new WindowManager(winP);

        deckListWinP->appendChild(winManP);

        startY += 3;
    }

    contentP->updateWindows();

    int currentDeckIndex = 0;

    while(true)
    {
        chtype ch = getch();

        if(ch == KEY_UP)
        {
            if(currentDeckIndex == 0)
            {

            }
            else
            {
                WindowManager* childP = deckListWinP->getChildAtIndex(currentDeckIndex);

                printCenter(childP->getBase(), deckContainerP->getDeckByIndex(currentDeckIndex)->name->c_str());

                currentDeckIndex--;

                childP = deckListWinP->getChildAtIndex(currentDeckIndex);

                WINDOW* winP = childP->getBase();
                wattron(winP, A_BOLD);
                printCenter(winP, deckContainerP->getDeckByIndex(currentDeckIndex)->name->c_str());
                wattroff(winP, A_BOLD);

                deckListWinP->updateWindows();
            }
            
        }
        else if(ch == KEY_DOWN)
        {

            if((currentDeckIndex + 1) == deckListWinP->numberOfChildren())
            {

            }
            else
            {
                WindowManager* childP = deckListWinP->getChildAtIndex(currentDeckIndex);

                printCenter(childP->getBase(), deckContainerP->getDeckByIndex(currentDeckIndex)->name->c_str());

                currentDeckIndex++;

                childP = deckListWinP->getChildAtIndex(currentDeckIndex);

                WINDOW* winP = childP->getBase();
                wattron(winP, A_BOLD);
                printCenter(winP, deckContainerP->getDeckByIndex(currentDeckIndex)->name->c_str());
                wattroff(winP, A_BOLD);

                deckListWinP->updateWindows();
            }
            
        }
        else if(ch == 27)
        {
            //Return when ESC
            return;
        }
        else if(ch == KEY_ENTER || ch == '\n')
        {
            deckPage(deckContainerP->getDeckByIndex(currentDeckIndex));
            return;
        }
        
    }
}


void TUI::deckPage(deck* deckP)
{
    WindowManager* contentP = base.getChildAtIndex(1);  //Index 1 is the content

    contentP->clearChildren();

    contentP->updateWindows();

    char buf[40];

    sprintf(buf, "%d", deckP->id);

    wmove(contentP->getBase(), 1, 2);


    wprintw(contentP->getBase(), buf);

    wmove(contentP->getBase(), 2, 2);

    wprintw(contentP->getBase(), deckP->name->c_str());

    contentP->updateWindows();


    chtype ch = getch();
}


void TUI::createDeckForm(DeckContainer* deckCoontainerP)
{
    WindowManager* contentP = base.getChildAtIndex(1);

    contentP->clearChildren();


    WindowManager* formBoxP = new WindowManager(contentP->y() + 2, 
                                                contentP->x() + 2,
                                                6, 
                                                50);

    contentP->appendChild(formBoxP);

    WINDOW* wP = formBoxP->getBase();

    simpleBox(wP);

    formBoxP->printCenter("Name of deck:");

    contentP->updateWindows();

    int c = 0;

    int startX = 2;

    std::string deckName;

    while(true)
    {
        c = getch();

        if(c == KEY_ENTER || c == '\n' || c == 10)
        {
            break;
        }
        else if(c == 27)
        {
            //if ESC return
            return;
        }
        else if(c == KEY_BACKSPACE || c == KEY_DC || c == 127 || c == 8 || c == '\b')
        {
            if(startX > 2)
            {
                wmove(wP, 2, --startX);
                waddch(wP, ' ');
                deckName = deckName.substr(0, deckName.size()-1);
            }
        }
        else
        {
            deckName += c;
            wmove(wP, 2, startX++);
            waddch(wP, c);
        }

        contentP->updateWindows();
    }

    deck* newDeckP = new deck();

    newDeckP->name = new std::string();
    
    newDeckP->name->assign(deckName);

    deckCoontainerP->addDeck(newDeckP);
    
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
