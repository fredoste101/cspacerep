#pragma once
#include <ncurses.h>
#include <string.h>
#include "windowManager.h"
#include "tuiConstants.h"
#include "../config/config.h"
#include "../timeUtils.h"
#include "../deck/deck.h"


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

        void printCenter(WINDOW* winP, const char* str);

        void simpleBox(WINDOW* winP);

        WINDOW* getBox(unsigned int posY, 
                       unsigned int posX, 
                       unsigned int height, 
                       unsigned int width);

    public:
        TUI();

        void showConfigurationScreen(programConfiguration* configP);

        void showStartScreen();

        void getOffsetInDayToStudy(programConfiguration* configP);

        void listDecks(DeckContainer* deckContainerP);

        void showMenu();

        void showHeader();

};


