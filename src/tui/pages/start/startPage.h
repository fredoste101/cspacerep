#include "../../tui.h"

#define NUM_OF_MAIN_MENU_CHOICES 2


typedef enum
{
  LIST_DECKS = 0,
  CREATE_DECK,
  LIST_NOTES,
  STUDY,
  FLASHCARD_STUDY,
  CREATE_NOTE,
  CHANGE_NOTE,
  DELETE_NOTE
} MAIN_MENU_CHOICES;

bool TUI::startPage()
{
    printMenuHeader(" - Main Menu");
    showMenu();

    base.updateWindows();

    char choice = getch();

    switch(choice)
    {
        case 27:    //ESC
        {
            deckContainerP->save();
            noteContainerP->save();

            return true;
        }
        break;

        case LIST_DECKS + 48:
        {
            //This should really have create deck as well. just a + in the list
            deckListPage(deckContainerP);
        }
        break;

        case CREATE_DECK + 48:
        {
            createDeckPage(deckContainerP);
        }
        break;


        default:
            fprintf(stderr, "\nERROR: unknown menu choice=%d\n", choice);
        break;
    }

    return false;
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


    const char* menuStringList[NUM_OF_MAIN_MENU_CHOICES] = 
    {
        "0 LIST DECKS",
        "1 CREATE DECK"
    };

    unsigned int i;

    for(i = 0; i < NUM_OF_MAIN_MENU_CHOICES; i++)
    {
        int startY = (menuStartY+2);
        int startX = (menuStartX+3) + i * 21;

        WindowManager* winManP = getBox(startY, startX, 3, 20);

        printCenter(winManP->getBase(), menuStringList[i]);

        contentP->appendChild(winManP);
    }

    contentP->updateWindows();
}