//File containing the definition of the deck-struct
//and helper functions for this.
#ifndef DECK_H_INCLUSION_PROTECTION
#define DECK_H_INCLUSION_PROTECTION

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "deck.h"


class DeckContainer
{
    private:
        std::vector<deck*>  deckList;
        std::fstream*       fileP;
        std::string         fileName;
        unsigned int        fileSize;
        unsigned int        nextDeckId;

        void readDeckList();


    public:
        DeckContainer();

        ~DeckContainer();

        void            setFile(std::string fileName);
        bool            load();
        void            save();
        void            addDeck(deck* deckP);
        deck*           getDeckByIndex(unsigned int index);
        bool            removeDeckByIndex(unsigned int index);
        unsigned int    numOfDecks();
    
    private:
        deck* createDefaultDeck();
};

#endif