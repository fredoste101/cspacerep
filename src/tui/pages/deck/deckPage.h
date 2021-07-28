#pragma once

#include "../../tui.h"


#define NUM_OF_DECK_OPTIONS 5

#define LIST_NOTE_INDEX 2
#define CREATE_NOTE_INDEX 3
#define REMOVE_DECK_INDEX 4

void TUI::deckPage(WindowManager* contentP, DeckContainer* deckContainerP, int deckIndex)
{

    deck* deckP = deckContainerP->getDeckByIndex(deckIndex);


    std::string stringList[NUM_OF_DECK_OPTIONS];

    char buf[40];

    sprintf(buf, "id: %d", deckP->id);

    stringList[0] = buf;
    stringList[1] = *deckP->name;
    stringList[2] = "[=] List notes";
    stringList[3] = "[+] Add note";
    stringList[4] = "[x] REMOVE DECK";

    WindowManager* deckMenuWinManP = initDeckPage(contentP, stringList);


   int currentMenuIndex = 0;

    while(true)
    {
        chtype ch = getch();

        switch(ch)
        {
            case KEY_UP:
            {
                if(currentMenuIndex == 0)
                {

                }
                else
                {
                    WindowManager* childP = deckMenuWinManP->getChildAtIndex(currentMenuIndex);

                    printCenter(childP->getBase(), stringList[currentMenuIndex].c_str());

                    currentMenuIndex--;

                    childP = deckMenuWinManP->getChildAtIndex(currentMenuIndex);

                    WINDOW* winP = childP->getBase();

                    wattron(winP, A_BOLD);
                    printCenter(winP, stringList[currentMenuIndex].c_str());
                    wattroff(winP, A_BOLD);

                    deckMenuWinManP->updateWindows();
                }
            }
            break;

            case KEY_DOWN:
            {
                if((currentMenuIndex + 1) == deckMenuWinManP->numberOfChildren())
                {

                }
                else
                {
                    WindowManager* childP = deckMenuWinManP->getChildAtIndex(currentMenuIndex);

                    printCenter(childP->getBase(), stringList[currentMenuIndex].c_str());

                    currentMenuIndex++;

                    childP = deckMenuWinManP->getChildAtIndex(currentMenuIndex);

                    WINDOW* winP = childP->getBase();
                    wattron(winP, A_BOLD);
                    childP->printCenter(stringList[currentMenuIndex]);
                    wattroff(winP, A_BOLD);

                    deckMenuWinManP->updateWindows();
                }
            }
            break;

            case 27:
                //ESC
                return;
            break;

            case KEY_ENTER:
            case '\n':
            {
                switch(currentMenuIndex)
                {  
                    case LIST_NOTE_INDEX:
                    {
                        noteListPage(deckIndex);
                        deckMenuWinManP = initDeckPage(contentP, stringList);
                        currentMenuIndex = 0;
                    }
                    break;

                    case CREATE_NOTE_INDEX:
                    {
                        note* tmpNote = createNotePage();

                        deckMenuWinManP = initDeckPage(contentP, stringList);
                        currentMenuIndex = 0;
                      

                        tmpNote->id     = 0;
                        tmpNote->type   = TEXT;
                        tmpNote->deckP  = deckContainerP->getDeckByIndex(deckIndex);
                        tmpNote->size   = sizeof(note);

                        tmpNote->deckP->numOfNotes++;
                        tmpNote->deckP->numOfNotesAllChildren++;

                        noteContainerP->addNote(deckIndex, tmpNote);
                        
                    }
                    break;

                    case REMOVE_DECK_INDEX:
                    {
                        //Cannot remove yet
                        //Create page for this. remove all children? remove all notes? etc etc must be taken care of
                        //deckContainerP->removeDeckByIndex(deckIndex);
                        return;
                    }
                    break;

                    default:

                    break;
                }
            }
            break;

            default:

            break;
        }
    }
}


WindowManager* TUI::initDeckPage(WindowManager* contentP, std::string* stringList)
{

    printMenuHeader(" - DECK");

    contentP->clearChildren();
    contentP->updateWindows();

    WindowManager* deckMenuWinManP = getBox(contentP->y() + 2, 2, 40, 44);

    WindowManager* winManPList[NUM_OF_DECK_OPTIONS];

    int winYStep = 2;

    for(unsigned int i = 0; i < NUM_OF_DECK_OPTIONS; i++)
    {
        winManPList[i] = getBox(deckMenuWinManP->y() + winYStep, deckMenuWinManP->x() + 2, 3, 40);

        if(i == 0)
        {
            wattron(winManPList[i]->getBase(), A_BOLD);
        }

        winManPList[i]->printCenter(stringList[i]);

        if(i == 0)
        {
            wattroff(winManPList[i]->getBase(), A_BOLD);
        }

        winYStep += 4;
    }


    deckMenuWinManP->appendChildren(winManPList, NUM_OF_DECK_OPTIONS);

    contentP->appendChild(deckMenuWinManP);

    contentP->updateWindows();

    return deckMenuWinManP;
}
