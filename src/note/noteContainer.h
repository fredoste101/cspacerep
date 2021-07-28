#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "note.h"

class NoteContainer
{
    private:
        std::string noteFileName;
        DeckContainer* deckContainerP;
        std::vector< std::vector<note*>* > deckNoteList;   ///< a list (one per deck) of lists (notes)
	
	std::vector<note*>* loadNoteData(unsigned int numOfNotes, std::fstream* noteFileP);

    public:
        NoteContainer(DeckContainer* dcP);
        ~NoteContainer();

        void setFileName(std::string fileName);

        void load();

        void save();

        void addDeck();

        void addNote(unsigned int deckIndex, note* noteToAddP);

        void removeNote(unsigned int deckIndex);    ///< @todo implement smartly

        std::vector<note*>* getNoteListByDeckIndex(unsigned int deckIndex);

        void removeNoteAtIndex(unsigned int index);
};
