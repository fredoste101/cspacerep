#include "deck.h"

DeckContainer::DeckContainer()
{
    fileP = NULL;

}

DeckContainer::~DeckContainer()
{
    if (fileP != NULL)
    {
        fileP->close();
    }
}


void DeckContainer::setFile(std::string fileName)
{
    fileP->open(fileName.c_str());

}

void DeckContainer::save()
{
    
}

void DeckContainer::addDeck(deck* deckP)
{

}

deck* DeckContainer::getDeckByIndex(unsigned int index)
{

}

unsigned int DeckContainer::getNumOfDecks()
{

}