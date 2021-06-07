#include "noteContainer.h"


NoteContainer::NoteContainer(DeckContainer* dcP)
{
    deckContainerP = dcP;
    unsigned int numOfDecks = dcP->numOfDecks();
    for(unsigned int i = 0; i < numOfDecks; i++)
    {
        deckNoteList.push_back(new std::vector<note*>());
    }
}


NoteContainer::~NoteContainer()
{
    //When destructing, save?
}


void NoteContainer::setFileName(std::string fileName)
{
    noteFileName.assign(fileName);
}


void NoteContainer::load()
{
    std::fstream noteFile(noteFileName.c_str(), std::ios::binary | std::ios::in);

    char buf[100];

    unsigned int numOfNotes = 0;

    noteFile.read(buf, sizeof(numOfNotes));
    
    memcpy(&numOfNotes, buf, sizeof(numOfNotes));

    unsigned int currentDeckIndex = 0;

    unsigned int currentDeckNumOfNotes = deckContainerP->getDeckByIndex(currentDeckIndex)->numOfNotes;


    for(unsigned int i = 0; i < numOfNotes; i++)
    {
        while(currentDeckNumOfNotes == 0)
        {
            currentDeckIndex++;
            fprintf(stderr, "currentDeckIndex: %u currentDeckNumOfNotes=%u", currentDeckIndex, currentDeckNumOfNotes);
            currentDeckNumOfNotes = deckContainerP->getDeckByIndex(currentDeckIndex)->numOfNotes;
        }

        note* loadedNoteP = new note();
        noteFile.read(buf, sizeof(note));
        memcpy(loadedNoteP, buf, sizeof(note));

        loadedNoteP->front = new std::string("Not what you ordered");
        loadedNoteP->back = new std::string("Not what you ordered back");

        loadedNoteP->deckP = deckContainerP->getDeckByIndex(currentDeckIndex);

        deckNoteList[currentDeckIndex]->push_back(loadedNoteP);


    }
}



/**
 * @brief Saves the notes to a file
 * 
 * ----------------------------------------
 * |  numOfnotes | noteLists | stringList |
 * ----------------------------------------
 * 
 */
void NoteContainer::save()
{
    std::fstream noteFile(noteFileName.c_str(), std::ios::binary | std::ios::trunc | std::ios::out);


    //Write number of notes

    unsigned int numOfNotes = 0;

    for(unsigned int i = 0; i < deckNoteList.size(); i++)
    {
        numOfNotes += deckNoteList[i]->size();
    }

    char writeBufferP[100];

    memcpy(writeBufferP, &numOfNotes, sizeof(unsigned int));

    noteFile.write(writeBufferP, sizeof(unsigned int));


    //Write notes

    for(unsigned int i = 0; i < deckNoteList.size(); i++)
    {
        std::vector<note*>* noteList = deckNoteList[i];

        for(unsigned int j = 0; j < noteList->size(); j++)
        {
            memcpy(writeBufferP, noteList->at(j), sizeof(*noteList->at(j)));

            noteFile.write(writeBufferP, sizeof(*noteList->at(j)));
        }
    }


    //Write strings
    for(unsigned int i = 0; i < deckNoteList.size(); i++)
    {
        std::vector<note*>* noteList = deckNoteList[i];

        for(unsigned int j = 0; j < noteList->size(); j++)
        {
            unsigned int frontLength    = sizeof(char) * noteList->at(j)->front->length();
            unsigned int backLength     = sizeof(char) * noteList->at(j)->back->length();

            memcpy(writeBufferP, noteList->at(j)->front->c_str(), frontLength);

            noteFile.write(writeBufferP, frontLength);

            memcpy(writeBufferP, noteList->at(j)->back->c_str(), backLength);

            noteFile.write(writeBufferP, backLength);
        }
    }
}


void NoteContainer::addNote(unsigned int deckIndex, note* noteToAddP)
{
    deckNoteList[deckIndex]->push_back(noteToAddP);
}


std::vector<note*>* NoteContainer::getNoteListByDeckIndex(unsigned int deckIndex)
{
    return deckNoteList[deckIndex];
}