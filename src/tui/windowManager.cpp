#include "windowManager.h"


WindowManager::WindowManager()
{
    return;
}


WindowManager::WindowManager(int posY, int posX, int sizeY, int sizeX)
{
    parentP = NULL;
    baseWinP = newwin(sizeY, sizeX, posY, posX);
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
        wclear(baseWinP);
        wrefresh(baseWinP);
        delwin(baseWinP);
    }
    else
    {
        endwin();
    }
    
}


void WindowManager::addStyle()
{

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
    childP->parentP = this;

    std::vector<WindowManager*>::iterator it;
    it = childList.begin() + index;

    childList.insert(it, childP);
}


void WindowManager::clear()
{
    clearChildren();
    wclear(baseWinP);
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
    childP->parentP = this;
    childList.push_back(childP);
}


/**
 * @depracated use numberOfChildren instead
 * 
 * @return unsigned int 
 */
unsigned int WindowManager::getNumOfChildren()
{
    return childList.size();
}


unsigned int WindowManager::numberOfChildren()
{
    return childList.size();
}


int WindowManager::height()
{
    return getmaxy(baseWinP);
}


int WindowManager::width()
{
    return getmaxx(baseWinP);
}


WindowManager* WindowManager::getChildAtIndex(unsigned int index)
{
    if(index < childList.size())
    {
        return childList[index];
    }
    else
    {
        return NULL;
    }
    
}


void WindowManager::setParent(WindowManager* inParentP)
{
    parentP = inParentP;
}


WINDOW* WindowManager::getBase()
{
    return baseWinP;
}


void WindowManager::setBase(WINDOW* baseP)
{
    baseWinP = baseP;
}


int WindowManager::y()
{
    return getbegy(baseWinP);
}


int WindowManager::x()
{
    return getbegx(baseWinP);
}


void WindowManager::printCenter(std::string msg)
{
    unsigned int maxY, maxX;

    getmaxyx(baseWinP, maxY, maxX);

    wmove(baseWinP, maxY/2, (maxX - msg.length())/2);

    wprintw(baseWinP, msg.c_str());
}