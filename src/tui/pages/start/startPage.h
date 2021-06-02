#include "../../tui.h"

void TUI::startPage()
{
    printMenuHeader(" - Main Menu");
    showMenu();

    base.updateWindows();
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

        WindowManager* winManP = getBox(startY, startX, 3, 20);

        printCenter(winManP->getBase(), menuStringList[i]);

        contentP->appendChild(winManP);
    }

    contentP->updateWindows();
}