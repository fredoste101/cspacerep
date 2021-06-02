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

    for(unsigned int i = 0; i < noteListP->size(); i++)
    {
        WindowManager* tmpWinManP = getBox(noteListWinManP->y() + yPosition, 
                                            noteListWinManP->x() + 2, 
                                            3, 
                                            noteListWinManP->width() - 4);

        yPosition += 4;

        tmpWinManP->printCenter(*(noteListP->at(i)->front));

        noteListWinManP->appendChild(tmpWinManP);                           
    }

    contentP->appendChild(noteListWinManP);

    contentP->updateWindows();

    int ch = getch();
    
}


