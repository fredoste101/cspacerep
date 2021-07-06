#pragma once

#include <ncurses.h>
#include <string.h>
#include <windowManager.h>
#include "tuiConstants.h"
#include "../config/config.h"
#include "../timeUtils.h"
#include "../deck/deck.h"
#include "../note/noteContainer.h"


void initMenuWindow(WINDOW* menuWinP);
void initHeaderWindow(WINDOW* headerWinP);
void simpleBox(WINDOW* winP);
void printCenter(WINDOW* winP, const char* str);

WINDOW* getBox(unsigned int posY, 
               unsigned int posX, 
               unsigned int height, 
               unsigned int width);


class TUI
{
    private:
        WindowManager base; ///< base window. In this case, it is stdscr

        DeckContainer* deckContainerP;
        NoteContainer* noteContainerP;

        void printCenter(WINDOW* winP, const char* str);

        void simpleBox(WindowManager* winManP);

        WindowManager* getBox(unsigned int posY, 
                              unsigned int posX, 
                              unsigned int height, 
                              unsigned int width);

        void deckPage(WindowManager* contentP, DeckContainer* deckContainerP, int deckIndex);

        void showMenu();

        WindowManager* getContent();

        WindowManager* getHeader();

        WindowManager* initDeckListPage(DeckContainer* deckContainerP, WindowManager* contentP);

        WindowManager* initDeckPage(WindowManager* contentP, std::string* stringList);

        WindowManager* initCreateNotePage();

        void noteListPage(unsigned int deckIndex);


        void printMenuHeader(std::string header);



    public:
        TUI();

        void showConfigurationScreen(programConfiguration* configP);

        void startPage();

        void getOffsetInDayToStudy(programConfiguration* configP);

        void deckListPage(DeckContainer* deckContainerP);

        void createDeckPage(DeckContainer* deckCoontainerP);

        void setDeckContainer(DeckContainer* dcP);

        void setNoteContainer(NoteContainer* ncP);


        void createNotePage();
        

};


