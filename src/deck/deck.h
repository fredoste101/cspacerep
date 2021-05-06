//File containing the definition of the deck-struct
//and helper functions for this.
#ifndef DECK_H_INCLUSION_PROTECTION
#define DECK_H_INCLUSION_PROTECTION

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

typedef struct deck deck;

typedef struct deck
{
    unsigned int    size;                   ///< if struct will change between versions
    std::string     name;                   ///< name of the deck
    deck*           parentP;                ///< parent deck
    unsigned int    numOfNotes;             ///< number of notes directly in this deck
    unsigned int    numOfNotesAllChildren;  ///< total number of notes in all children
    unsigned int    reserved;               ///< reserved for future use
} deck;


class DeckContainer
{
    private:
        std::vector<deck>   list;
        std::fstream*       fileP;

    public:
        DeckContainer();

        ~DeckContainer();

        void            setFile(std::string fileName);
        void            load();
        void            save();
        void            addDeck(deck* deckP);
        deck*           getDeckByIndex(unsigned int index);
        unsigned int    getNumOfDecks();
    
};

#endif