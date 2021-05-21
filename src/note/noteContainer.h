#include <string>
#include <iostream>
#include <fstream>

#include "note.h"

class NoteContainer
{
    private:
        std::string noteFileName;
        std::vector<std::vector<note*>> deckNoteList;

    public:
        NoteContainer();
        ~NoteContainer();


        void addNote(unsigned int deckIndex, note* noteToAddP);
}