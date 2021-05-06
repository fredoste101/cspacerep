#pragma once
#include <ncurses.h>
#include <string.h>
#include "windowManager.h"
#include "tuiConstants.h"


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

    public:
        TUI();

        void showStartScreen();


    

};


