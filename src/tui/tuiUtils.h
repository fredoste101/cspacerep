#pragma once
#include <ncurses.h>
#include <string.h>
#include "tuiConstants.h"


void initMenuWindow(WINDOW* menuWinP);
void initHeaderWindow(WINDOW* headerWinP, int maxX);
void simpleBox(WINDOW* winP);