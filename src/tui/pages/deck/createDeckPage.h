#include "../../tui.h"

void TUI::createDeckPage(DeckContainer* deckCoontainerP)
{
    WindowManager* contentP = base.getChildAtIndex(1);

    contentP->clearChildren();


    WindowManager* formBoxP = new WindowManager(contentP->y() + 2, 
                                                contentP->x() + 2,
                                                6, 
                                                50);

    contentP->appendChild(formBoxP);

    WINDOW* wP = formBoxP->getBase();

    simpleBox(formBoxP);

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