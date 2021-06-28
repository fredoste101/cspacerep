#include "../../tui.h"


typedef enum inputWin
{
    NOTE_FRONT_INPUT = 0,
    NOTE_BACK_INPUT
} inputWin;

void TUI::createNotePage()
{
    WindowManager* createNoteContainerWinManP = initCreateNotePage();

    inputWin currentWin = NOTE_FRONT_INPUT;

    std::string front;
    std::string back;


    wmove(createNoteContainerWinManP->getBase(), 1, 1);


    int inputChar = getch();

    fprintf(stderr, "\nKEY_ENTER=%d", KEY_ENTER);

    while(inputChar != KEY_ENTER && inputChar != '\n')
    {
        switch(inputChar)
        {

            case KEY_BTAB:
            {
                fprintf(stderr, "\nKEY_BTAB PRESSED");
            }
            break;

            case '\t':
            {
                fprintf(stderr, "\nTAB PRESSED");
            }
            break;

            default:
            {
                waddch(createNoteContainerWinManP->getBase(), inputChar);
                createNoteContainerWinManP->updateWindows();
            }
            break;
        }

        fprintf(stderr, "\n%d", inputChar);

        inputChar = getch();
    }   

    

}


WindowManager* TUI::initCreateNotePage()
{
    printMenuHeader(" - Create Note");

    WindowManager* contentP = getContent();

    contentP->clearChildren();
    contentP->updateWindows();

    WindowManager* createNoteWinManP = getBox(contentP->y() + 2, 
                                              contentP->x() + 2, 
                                              contentP->height() - 4, 
                                              contentP->width() - 4);


    WindowManager* frontInputWinManP = getBox(createNoteWinManP->y() + 1, 
                                              createNoteWinManP->x() + 1, 
                                              (createNoteWinManP->height() - 2) / 2, 
                                              createNoteWinManP->width() - 2);

    WindowManager* backInputWinManP = getBox(createNoteWinManP->y() + (createNoteWinManP->height() - 2) / 2 + 1, 
                                              createNoteWinManP->x() + 1, 
                                              (createNoteWinManP->height() - 2) / 2, 
                                              createNoteWinManP->width() - 2);

    
    createNoteWinManP->appendChild(frontInputWinManP);
    createNoteWinManP->appendChild(backInputWinManP);


    contentP->appendChild(createNoteWinManP);

    contentP->updateWindows();
    
    return createNoteWinManP;
}