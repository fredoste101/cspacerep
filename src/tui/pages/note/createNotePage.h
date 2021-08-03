#include "../../tui.h"


typedef enum inputWin
{
    NOTE_FRONT_INPUT = 0,
    NOTE_BACK_INPUT
} inputWin;


note* TUI::createNotePage()
{
    WindowManager* createNoteContainerWinManP = initCreateNotePage();

    inputWin currentWinIndex = NOTE_FRONT_INPUT;
    std::string* noteStringList[2];
    noteStringList[0] = new std::string();
    noteStringList[1] = new std::string();

    wmove(createNoteContainerWinManP->getBase(), 1, 1);

    int inputChar = getch();

    while(inputChar != KEY_ENTER && inputChar != '\n')
    {
        switch(inputChar)
        {
            case KEY_BTAB:
            case '\t':
            {
                if (currentWinIndex == NOTE_FRONT_INPUT)
                {
                    currentWinIndex = NOTE_BACK_INPUT;
                }
                else
                {
                    currentWinIndex = NOTE_FRONT_INPUT;
                }
            }
            break;

            case KEY_BACKSPACE:
            case KEY_DC:
            case 127:
            case 8:
            {
                std::string* currentStringP = noteStringList[currentWinIndex];

                if (currentStringP->size() > 0)
                {
                    currentStringP->pop_back();

                    WindowManager* currentWinManP = createNoteContainerWinManP->getChildAtIndex(currentWinIndex);

                    WINDOW* tmpWin = currentWinManP->getBase();

                    int x, y;

                    getyx(tmpWin, y, x);

                    wmove(tmpWin, y, x - 1);
                    waddch(tmpWin, ' ');
                    wmove(tmpWin, y, x - 1);

                    currentWinManP->updateWindows();
                }
            }
            break;

            case 27:
            {
                return NULL;
            }


            default:
            {

		        WindowManager* currentWinManP = createNoteContainerWinManP->getChildAtIndex(currentWinIndex);
		
		        *noteStringList[currentWinIndex] += inputChar;

                waddch(currentWinManP->getBase(), inputChar);
                        createNoteContainerWinManP->updateWindows();
            }
            break;
        }

        inputChar = getch();
    }   

    note* noteP 	= new note();
    noteP->front    = noteStringList[0];
    noteP->back     = noteStringList[1];

    return noteP;
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

    wmove(frontInputWinManP->getBase(), 1, 2);    
    wmove(backInputWinManP->getBase(), 1, 2);    

    createNoteWinManP->appendChild(frontInputWinManP);
    createNoteWinManP->appendChild(backInputWinManP);


    contentP->appendChild(createNoteWinManP);

    contentP->updateWindows();
    
    return createNoteWinManP;
}
