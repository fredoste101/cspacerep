#include "../../tui.h"


void TUI::deckPage(WindowManager* contentP, deck* deckP)
{

    contentP->clearChildren();

    contentP->updateWindows();

    char buf[40];

    sprintf(buf, "%d", deckP->id);

    wmove(contentP->getBase(), 1, 2);


    wprintw(contentP->getBase(), buf);

    wmove(contentP->getBase(), 2, 2);

    wprintw(contentP->getBase(), deckP->name->c_str());

    contentP->updateWindows();

    //Add remove button


    chtype ch = getch();
}
