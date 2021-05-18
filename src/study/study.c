#include "study.h"


void startStudy(time_t       studyTime, 
                noteList*    noteListP)
{
    note* notesToStudy[MAX_NOTES_TO_STUDY];

    unsigned int numOfNotesToStudy = getNotesToStudy(noteListP, 
                                                     studyTime, 
                                                     notesToStudy, 
                                                     MAX_NOTES_TO_STUDY);

    int numOfActiveNotes = 0;

    if(numOfNotesToStudy > 0)
    {
        do
        {
            numOfActiveNotes = studyLoop(numOfNotesToStudy, 
                                         notesToStudy, 
                                         studyTime);

        } while(numOfActiveNotes > 0);
    }
    else
    {
        //printf("\nNo notes to study\n");
    }
}


int studyLoop(unsigned int  numOfNotesToStudy, 
              note*         notesToStudy[MAX_NOTES_TO_STUDY], 
              time_t        studyTime)
{
    int numOfActiveNotes = 0;

    unsigned int i = 0;

    for(i = 0; i < numOfNotesToStudy; i++)
    {
        note* currentNoteP = notesToStudy[i];

        if(currentNoteP == NULL)
        {
            continue;
        }

        numOfActiveNotes++;

        int q = studyNote(currentNoteP);


        switch(q)
        {
            case 0:
                continue;
            break;
            case 1:
                currentNoteP->interval = 0;
            break;
            case 2:
            case 3:
            case 4:

                q++; //To fit supermemos rankings (0-5), of which 3,4 and 5 are good

                
                if(currentNoteP->interval == 0)
                {
                    currentNoteP->interval      = 1;
                }
                else if(currentNoteP->interval == 1)
                {
                    currentNoteP->ef            = calculateNewEf(currentNoteP->ef, q);;
                    currentNoteP->interval      = 6;
                }
                else
                {
                    currentNoteP->interval      = currentNoteP->interval * currentNoteP->ef;
                    currentNoteP->ef            = calculateNewEf(currentNoteP->ef, q);;
                }

                currentNoteP->nextStudyTime = studyTime + SECONDS_IN_DAY * currentNoteP->interval - 1; //Add interval num of seconds
                                                                                                    // -1 is to get some time buffer for check
                notesToStudy[i] = NULL; //This one is done
            break;
            case 9:
                return -1;
            break;
            default:
                //printf("unknown option %u. skipping...\n", q);    
            break;
        }

    }

    return numOfActiveNotes;
}


void showTextFront(WINDOW* frontWinP, char* front)
{
    wmove(frontWinP, 2,3);
    wprintw(frontWinP, front);
    wrefresh(frontWinP);
}


void showTextBack(WINDOW* backWinP, char* back)
{
    wmove(backWinP, 2, 3);
    wprintw(backWinP, back);
    wrefresh(backWinP);
}


void showInBrowser(const char* uri)
{
    char buffer[1050];
    sprintf(buffer, "firefox %s &> /dev/null", uri);
    system(buffer);
}


void showQualityInput(WINDOW* qualityWinP)
{
    wmove(qualityWinP, 1, 1);
    wprintw(qualityWinP, "0 (Skip) - 1 (again) - 2 (ok) - 3 (Good) - 4 (Easy) - 9 (Exit)");
    wrefresh(qualityWinP);
}


void clearAllStudyWindows(WINDOW* frontWinP, WINDOW* backWinP, WINDOW* qualityWinP)
{
    werase(frontWinP);
    werase(backWinP);
    werase(qualityWinP);

    wrefresh(frontWinP);
    wrefresh(backWinP);
    wrefresh(qualityWinP);

    delwin(backWinP);
    delwin(frontWinP);
    delwin(qualityWinP);
}


int getQualityInput()
{
    int gottenQualityInput = 0;
    int quality;

    while(!gottenQualityInput)
    {
        quality = getch() - 48;
        if(quality <= 5 && quality >= 1 || quality == 9)
        {
            gottenQualityInput = 1;
        }
    }

    return quality;
}


int studyNote(note* currentNoteP)
{
    char buffer[1050];

    int maxY, maxX;
    
    int qualityWinHeight = 5;

    getmaxyx(stdscr, maxY, maxX);

    int contentHeight = ((maxY - HEADER_HEIGHT - qualityWinHeight - 1) / 2);

    WINDOW* frontWinP   = newwin(contentHeight, maxX-2, HEADER_HEIGHT+1, 1);
    WINDOW* backWinP    = newwin(contentHeight, maxX-2, HEADER_HEIGHT + contentHeight + 1, 1);

    WINDOW* qualityWinP = newwin(qualityWinHeight, maxX-2, (HEADER_HEIGHT + contentHeight * 2) + 1, 1);

    refresh();

    switch(currentNoteP->type)
    {
        case TEXT:
            showTextFront(frontWinP, currentNoteP->front.data);

            getch(); //Any keypress to show answer

            showTextBack(backWinP, currentNoteP->back.data);

            showQualityInput(qualityWinP);

        break;

        case BROWSER:
            showInBrowser(currentNoteP->front.data);

            getch(); //Any keypress to show answer

            showInBrowser(currentNoteP->back.data);
        break;

        case TEXT_BROWSER:
            showTextFront(frontWinP, currentNoteP->front.data);

            getch();

            showQualityInput(qualityWinP);

            showInBrowser(currentNoteP->back.data);
        break;

        default:
            //printf("oh noes :(\n");
        break;
    }

    int quality = getQualityInput();
     
    clearAllStudyWindows(frontWinP, backWinP, qualityWinP);


    return quality;
}
