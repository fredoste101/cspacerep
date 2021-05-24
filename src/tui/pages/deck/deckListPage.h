#include "../../tui.h"

/**
 * @brief shows deck list
 */ 
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

    simpleBox(deckListWinP);

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

        WindowManager* winManP = getBox(startY, 
                              startX, 
                              deckListElementHeight, 
                              deckListelementWidth);

        WINDOW* winP = winManP->getBase();


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
            deckPage(contentP, deckContainerP->getDeckByIndex(currentDeckIndex));
            return;
        }
        
    }
}
