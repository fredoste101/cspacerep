#include "study.h"


void study(time_t       studyTime, 
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
                //printf("Skipping...\n");
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
            case '9':
                return -1;
            break;
            default:
                //printf("unknown option %u. skipping...\n", q);    
            break;
        }

    }

    return numOfActiveNotes;
}

int studyNote(note* currentNoteP)
{
    char buffer[1050];

    int maxY, maxX;
    
    int qualityWinHeight = 10;

    getmaxyx(stdscr, maxY, maxX);

    int contentHeight = (maxY - HEADER_HEIGHT)/2 - qualityWinHeight;

    WINDOW* frontWinP   = newwin(contentHeight, maxX, HEADER_HEIGHT, 0);
    WINDOW* backWinP    = newwin(contentHeight, maxX, HEADER_HEIGHT + contentHeight, 0);
    WINDOW* qualityWinP = newwin(qualityWinHeight, maxX, HEADER_HEIGHT+contentHeight*2, 0);

    refresh();

    switch(currentNoteP->type)
    {
        case TEXT:
            simpleBox(frontWinP, contentHeight, maxX);
            wmove(frontWinP, 2,3);
            wprintw(frontWinP, currentNoteP->front);
            wrefresh(frontWinP);

            getch();

            simpleBox(backWinP, contentHeight, maxX);
            wmove(backWinP, 2, 3);
            wprintw(backWinP, currentNoteP->back);
            wrefresh(backWinP);

            simpleBox(qualityWinP, qualityWinHeight, maxX);
            wmove(qualityWinP, 1, 1);
            wprintw(qualityWinP, "0 - 1 - 2 - 3 - 4");
            wrefresh(qualityWinP);

        break;

        case BROWSER:
            ;
            sprintf(buffer, "firefox %s &> /dev/null", currentNoteP->front);
            system(buffer);

            printf("(SPACE) show answer (opens another web page)");
            while(fgetc(stdin) != ' ');

            sprintf(buffer, "firefox %s &> /dev/null", currentNoteP->back);
            system(buffer);
        break;

        case TEXT_BROWSER:
            simpleBox(frontWinP, contentHeight, maxX);
            wmove(frontWinP, 2,3);
            wprintw(frontWinP, currentNoteP->front);
            wrefresh(frontWinP);

            getch();

            simpleBox(qualityWinP, qualityWinHeight, maxX);
            wmove(qualityWinP, 1, 1);
            wprintw(qualityWinP, "0 - 1 - 2 - 3 - 4");
            wrefresh(qualityWinP);

            
            sprintf(buffer, "nohup &> /dev/null firefox %s &> /dev/null", currentNoteP->back);
            system(buffer);
        break;

        default:
            //printf("oh noes :(\n");
        break;
    }

    int gottenInput = 0;
    int quality;

    while(!gottenInput)
    {
        quality = getch() - 48;
        if(quality <= 5 && quality >= 1 || quality == 9)
        {
            gottenInput = 1;
        }
    }
     

    werase(frontWinP);
    werase(backWinP);
    werase(qualityWinP);

    wrefresh(frontWinP);
    wrefresh(backWinP);
    wrefresh(qualityWinP);

    delwin(backWinP);
    delwin(frontWinP);
    delwin(qualityWinP);

    return quality;
}