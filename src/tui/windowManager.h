#ifndef WINDOW_MANAGER_INCLUSION_PROTECTION
#define WINDOW_MANAGER_INCLUSION_PROTECTION

#include <ncurses.h>
#include <vector>

class WindowManager
{
    private:
        WINDOW*                     baseWinP;
        WindowManager*              parent;
        std::vector<WindowManager*> childList;
        

    public:
        WindowManager();
        WindowManager(WINDOW* baseP);

        ~WindowManager();

        int height();
        int width();


        WINDOW* getBase();

        void setBase(WINDOW* baseP);

        void updateWindows();

        void appendChild(WindowManager* child);

        void insertChild(unsigned int index, WindowManager* child);

        WindowManager* getChildAtIndex(unsigned int index);

        unsigned int getNumOfChildren();

        void clearChildren();
};


#endif