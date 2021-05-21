
#include "deck.h"
#include <stdlib.h>
#include <string.h>


DeckContainer::DeckContainer()
{
    nextDeckId = 0;
    fileP = new std::fstream();
}


DeckContainer::~DeckContainer()
{
    if (fileP != NULL)
    {
        fileP->close();
    }
}


void DeckContainer::setFile(std::string inFileName)
{
    fileName.assign(inFileName);

    fileP->open(fileName.c_str(), 
                std::ios::in | std::ios::out | std::ios::binary);
  

    if(!fileP->is_open())
    {
        fileSize = 0;
        fileP->open(fileName.c_str(), 
                    std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    }
    else
    {
        unsigned int begin = fileP->tellg();

        fileP->seekg(0, std::ios::end);
        unsigned int end = fileP->tellg();

        fileSize = end - begin;

        fileP->seekg(0, std::ios::beg);
    }

    fileP->close();
}


/**
 * @brief save decks to a file
 * 
 *  *******************************************************
 *  | numOfDecks | deckList | stringList | relationshipBm |
 *  *******************************************************
 * 
 *  numOfDecks - total number of decks, to know how many to read in. unsigned int (8 bytes)
 * 
 *  deckLiss - just like it sounds. @see deck
 * 
 *  stringList - each deck has a string in it. these strings are stored here as C-strings (ends with '\0')
 * 
 *  relationShipBm - A number of bitmaps that indicates children for given deck. Is in same order as deckList and mapps 1-1.
 *                   each entry is as big as number of decks (not optimal but easy implementation) in short ints. 
 * 
 */
void DeckContainer::save()
{
    fileP->open(fileName.c_str(), 
                std::ios::out | std::ios::binary);

    unsigned int numOfDecks = deckList.size();

    fprintf(stderr, "fileName=%s", fileName.c_str());

    char buffer[100];

    //Save numOfDecks

    memset(buffer, 0, sizeof(char) * 100);

    memcpy(buffer, &numOfDecks, sizeof(numOfDecks));

    fileP->write(&buffer[0], sizeof(numOfDecks));


    //Save deckList

    for(unsigned int i = 0; i < numOfDecks; i++)
    {
        memset(buffer, 0, sizeof(char) * 100);

        memcpy(buffer, deckList[i], sizeof(*deckList[i]));

        fileP->write(&buffer[0], sizeof(*deckList[i]));
    }


    //Save stringList

    for(unsigned int i = 0; i < numOfDecks; i++)
    {
        memset(buffer, 0, sizeof(char) * 100);

        size_t stringLength = sizeof(char) * (deckList[i]->name->length() + 1); //Including '\0'

        memcpy(buffer, deckList[i]->name->c_str(), stringLength);

        ///! @TODO What if stringLength is bigger than buffer?
        fileP->write(&buffer[0], stringLength);
    }

    fileP->close();

}


bool DeckContainer::load()
{
    if(fileSize == 0)
    {
        deck* defaultDeckP = createDefaultDeck();

        addDeck(defaultDeckP);
    }
    else
    {
        fileP->open(fileName.c_str(), 
                    std::ios::in | std::ios::binary);
        
        readDeckList();

        //Then there's the tricky part... strings...
        char buf[100];


        unsigned int stringIndex = 0;

        bool readingStrings = true;

        std::string currentString = "";

        std::streamsize numOfBytesRead;

        do
        {

            memset(buf, 0, sizeof(char) * 100);

            fileP->read(buf, sizeof(char) * 100);

            numOfBytesRead = fileP->gcount();

            if(readingStrings)
            {
                for(unsigned int i = 0; i < numOfBytesRead; i++)
                {
                    if(buf[i] == '\0')
                    {

                        deckList[stringIndex]->name = new std::string();

                        deckList[stringIndex]->name->assign(currentString);

                        stringIndex++;

                        if(stringIndex >= deckList.size())
                        {
                            //All decks have gotten their strings
                            readingStrings = false;
                            break;
                        }

                        currentString = "";
                    }
                    else
                    {
                        currentString += buf[i];
                    }
                }
            }
            else
            {
                //read relations
            }

        } 
        while (numOfBytesRead == (sizeof(char) * 100));

        fileP->close();

    }   

    
    return true; 
}


void DeckContainer::readDeckList()
{
    unsigned int numOfDecks = 0;

    char buf[100];

    //Read numOfDecks

    memset(buf, 0, sizeof(char)*100);

    fileP->read(buf, 4);

    memcpy(&numOfDecks, buf, sizeof(numOfDecks));


    //Read deckList

    for(unsigned int i = 0; i < numOfDecks; i++)
    {
        memset(buf, 0, sizeof(char)*100);

        deck* tmpDeckP = new deck;

        fileP->read(buf, sizeof(deck));

        memcpy(tmpDeckP, buf, sizeof(deck));

        if (nextDeckId <= tmpDeckP->id)
        {
            nextDeckId = tmpDeckP->id;
        }

        deckList.push_back(tmpDeckP);
    }

    nextDeckId++;
}


void DeckContainer::addDeck(deck* deckP)
{
    deckP->id                    = nextDeckId;
    deckP->numOfNotes            = 0;
    deckP->numOfNotesAllChildren = 0;
    deckP->size                  = sizeof(deck);

    nextDeckId++;
    
    deckList.push_back(deckP);
}


deck* DeckContainer::getDeckByIndex(unsigned int index)
{
    if(index < deckList.size())
    {
        return deckList[index];
    }
    else
    {
        return NULL;
    }

    
}


unsigned int DeckContainer::numOfDecks()
{
    return deckList.size();
}


deck* DeckContainer::createDefaultDeck()
{
    deck* defaultDeckP = new deck;

    defaultDeckP->name                  = new std::string("Default");
    defaultDeckP->id                    = nextDeckId;
    defaultDeckP->size                  = sizeof(*defaultDeckP);
    defaultDeckP->parentP               = NULL;
    defaultDeckP->numOfNotes            = 0;
    defaultDeckP->numOfNotesAllChildren = 0;
    
    return defaultDeckP;
}