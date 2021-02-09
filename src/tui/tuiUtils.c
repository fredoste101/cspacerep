#include "tuiUtils.h"

void initMenuWindow(WINDOW* menuWinP, int maxY, int maxX)
{
    simpleBox(menuWinP, maxY - HEADER_HEIGHT, maxX);

    wmove(menuWinP, 1, 3);
    wprintw(menuWinP, "0. Quit");
    wmove(menuWinP, 2, 3);
    wprintw(menuWinP, "1. List Notes");
    wmove(menuWinP, 3, 3);
    wprintw(menuWinP, "2. Study");
    wmove(menuWinP, 4, 3);
    wprintw(menuWinP, "3. Flashcard Study");
    wmove(menuWinP, 5, 3);
    wprintw(menuWinP, "4. Create Note");
    wmove(menuWinP, 6, 3);
    wprintw(menuWinP, "5. Change Note");
    wmove(menuWinP, 7, 3);
    wprintw(menuWinP, "6. Remove Note");
    wrefresh(menuWinP);
}


void initHeaderWindow(WINDOW* headerWinP, int maxX)
{

    simpleBox(headerWinP, HEADER_HEIGHT, maxX);

    wmove(headerWinP, HEADER_HEIGHT/2, (maxX - strlen("CSPACEREP"))/2);

    wprintw(headerWinP, "CSPACEREP");
}


void simpleBox(WINDOW* winP, int maxY, int maxX)
{
    unsigned int i = 0;
    unsigned int j = 0;


    wmove(winP, 0, 0);

    for(j = 0; j < maxX; j++)
    {
        waddch(winP, '-');
    }

    wmove(winP, maxY - 1, 0);

    for(j = 0; j < maxX; j++)
    {
        waddch(winP, '-');
    }

    wmove(winP, 0, 0);
    for(j = 0; j < maxY; j++)
    {
        waddch(winP, '|');
        wmove(winP, j, 0);
    }

    wmove(winP, 0, maxX - 1);
    for(j = 0; j < maxY; j++)
    {
        waddch(winP, '|');
        wmove(winP, j, maxX - 1);
    }


    /* Corners */
    wmove(winP, 0, 0);
    waddch(winP, '+');

    wmove(winP, 0, maxX - 1);
    waddch(winP, '+');

    wmove(winP, maxY - 1, 0);
    waddch(winP, '+');

    wmove(winP, maxY - 1, maxX - 1);
    waddch(winP, '+');
}


