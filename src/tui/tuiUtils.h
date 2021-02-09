#pragma once
#include <ncurses.h>
#include <string.h>
#include "tuiConstants.h"


void initMenuWindow(WINDOW* menuWinP, int maxY, int maxX);
void initHeaderWindow(WINDOW* headerWinP, int maxX);
void simpleBox(WINDOW* winP, int maxY, int maxX);