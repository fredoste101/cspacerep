#pragma once 

#include "../../tui.h"


void TUI::noteListPage(unsigned int deckIndex)
{
    WindowManager* contentP = getContent();

    contentP->clearChildren();

    WindowManager* noteListWinManP = getBox(contentP->y() + 2, 
                                            2, 
                                            contentP->height() - 4, 
                                            contentP->width() - 4);


    std::vector<note*>* noteListP = noteContainerP->getNoteListByDeckIndex(deckIndex);


    int yPosition = 2;

    unsigned int currentIndex = 0;

    for(unsigned int i = 0; i < noteListP->size(); i++)
    {
        

        WindowManager* entryWinManP = getBox(noteListWinManP->y() + yPosition, 
                                            noteListWinManP->x() + 2, 
                                            5, 
                                            noteListWinManP->width() - 4);

        const int idFieldWidth = 10;

        WindowManager* idFieldWinManP = getBox(noteListWinManP->y() + yPosition + 1, 
                                               noteListWinManP->x() + 2 + 1, 
                                               3, 
                                               idFieldWidth);

        WindowManager* nameFieldWinManP = getBox(noteListWinManP->y() + yPosition + 1, 
                                            noteListWinManP->x() + 2 + 1 + idFieldWidth, 
                                            3, 
                                            noteListWinManP->width() - 4 - idFieldWidth - 2);

        entryWinManP->appendChild(idFieldWinManP);
        entryWinManP->appendChild(nameFieldWinManP);

        if(i == currentIndex)
        {
            wattron(idFieldWinManP->getBase(), A_BOLD);
            idFieldWinManP->attributeOn(A_BOLD);
        }

        char tmpString[6];

        sprintf(tmpString, "%d", noteListP->at(i)->id);

        if(i == currentIndex)
        {
            wattroff(idFieldWinManP->getBase(), A_BOLD);
            idFieldWinManP->attributeOff(A_BOLD);
        }

        idFieldWinManP->printCenter(tmpString);

        




        nameFieldWinManP->printCenter(*(noteListP->at(i)->front));

        

        yPosition += 6;

        

        noteListWinManP->appendChild(entryWinManP);                           
    }

    contentP->appendChild(noteListWinManP);

    contentP->updateWindows();

    int ch = getch();

    while(ch != 27)
    {

        switch(ch)
        {
            case KEY_UP:
            {
                if(currentIndex != 0)
                {
                    currentIndex--;
                }
            }
            break;

            case KEY_DOWN:
            {
                if(currentIndex != noteListP->size() - 1)
                {
                    currentIndex++;
                }
            }
            break;

            default:
            {

            }
            break;
        }

        ch = getch();
    }
    
}


