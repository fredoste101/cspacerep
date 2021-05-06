#include "tuiUtils.h"

void initMenuWindow(WINDOW* menuWinP)
{
    simpleBox(menuWinP);

    wrefresh(menuWinP);

    int menuStartX, menuStartY;

    getbegyx(menuWinP, menuStartY, menuStartX);

    const char* menuStringList[2] = {"0 QUIT", "1 LIST DECKS"};

    WINDOW* menuWindowList[2];

    unsigned int i;

    for(i = 0; i < 2; i++)
    {
        int startY = (menuStartY+2);
        int startX = (menuStartX+3) + i * 21;

        menuWindowList[i] = getBox(startY, startX, 3, 20);
        wmove(menuWindowList[i], 1, 1);
        printCenter(menuWindowList[i], menuStringList[i]);
        wrefresh(menuWindowList[i]);
    }
    

    refresh();
}


void initHeaderWindow(WINDOW* headerWinP)
{
    simpleBox(headerWinP);
    printCenter(headerWinP, "*** CSPACEREP ***");
}


WINDOW* getBox(unsigned int posY, 
               unsigned int posX, 
               unsigned int height, 
               unsigned int width)
{
    WINDOW* win = newwin(height, 
                         width, 
                         posY, 
                         posX);

    simpleBox(win);

    return win;
}


void printCenter(WINDOW* winP, const char* str)
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
void simpleBox(WINDOW* winP)
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
}

void TUI::showStartScreen()
{
    int maxY;
    int maxX;

    getmaxyx(stdscr, maxY, maxX);

    

    WINDOW* headerWinP  = newwin(HEADER_HEIGHT, maxX, 0, 0);
    WINDOW* contentWinP    = newwin(maxY - HEADER_HEIGHT, maxX, HEADER_HEIGHT, 0);

    WindowManager* headerP = new WindowManager(headerWinP);
    WindowManager* contentP = new WindowManager(contentWinP);

    base.appendChild(headerP);
    base.appendChild(contentP);

    initHeaderWindow(headerWinP);
    initMenuWindow(contentWinP);

    base.updateWindows();
}

