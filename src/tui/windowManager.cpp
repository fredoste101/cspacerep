#include "windowManager.h"


WindowManager::WindowManager()
{
    return;
}


WindowManager::WindowManager(WINDOW* baseP)
{
    baseWinP = baseP;
}


WindowManager::~WindowManager()
{
    clearChildren();
    refresh();

    if(baseWinP != stdscr)
    {
        delwin(baseWinP);
    }
    else
    {
        endwin();
    }
    
}

void WindowManager::updateWindows()
{
    wrefresh(baseWinP);

    unsigned int i;

    for(i = 0; i < childList.size(); i++)
    {
        childList[i]->updateWindows();
    }

    refresh();
}


void WindowManager::insertChild(unsigned int index, WindowManager* childP)
{
    std::vector<WindowManager*>::iterator it;
    it = childList.begin() + index;

    childList.insert(it, childP);
}


void WindowManager::clearChildren()
{
    unsigned int i = 0;

    for(i = 0; i < childList.size(); i++)
    {
        delete childList[i];
    }

    childList.clear();
}


void WindowManager::appendChild(WindowManager* childP)
{
    childList.push_back(childP);
}


unsigned int WindowManager::getNumOfChildren()
{
    return childList.size();
}