#include "../../tui.h"

void TUI::startPage()
{
    showHeader();
    showMenu();

    base.updateWindows();
}


void TUI::showHeader()
{
    WindowManager* headerP = base.getChildAtIndex(0);  //Index 0 is the header

    WINDOW* headerWinP = headerP->getBase();

    simpleBox(headerP);
    printCenter(headerWinP, "*** CSPACEREP ***");
}


void TUI::showMenu()
{
    WindowManager* contentP = base.getChildAtIndex(1);  //Index 1 is the content

    contentP->clearChildren();

    simpleBox(contentP);

    contentP->updateWindows();

    int menuStartX, menuStartY;

    menuStartX = contentP->x();
    menuStartY = contentP->y();


    const char* menuStringList[3] = 
    {
        "0 QUIT", 
        "1 LIST DECKS",
        "2 CREATE DECK"
    };

    unsigned int i;

    for(i = 0; i < 3; i++)
    {
        int startY = (menuStartY+2);
        int startX = (menuStartX+3) + i * 21;

        fprintf(stderr, "\nstartY=%d startX=%d", startY, startX);

        WindowManager* winManP = getBox(startY, startX, 3, 20);

        printCenter(winManP->getBase(), menuStringList[i]);

        contentP->appendChild(winManP);
    }

    contentP->updateWindows();
}