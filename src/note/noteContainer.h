#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "note.h"

class NoteContainer
{
    private:
        std::string noteFileName;
        std::vector< std::vector<note*>* > deckNoteList;   ///< a list (one per deck) of lists (notes)

    public:
        NoteContainer(unsigned int numOfDecks);
        ~NoteContainer();

        void setFileName(std::string fileName);

        void load();

        void save();

        void addNote(unsigned int deckIndex, note* noteToAddP);

        std::vector<note*>* getNoteListByDeckIndex(unsigned int deckIndex);

        void removeNoteAtIndex(unsigned int index);
};