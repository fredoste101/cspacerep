#ifndef CSPACEREP_CONFIG_INCLUSION_PROTECTION
#define CSPACEREP_CONFIG_INCLUSION_PROTECTION


#include <string>
#include <iostream>
#include <fstream>

#include "../deck/deckContainer.h"
#include "../note/note.h"


#define VERSION 0


typedef struct programMetaData
{
    unsigned int sizeOfConfigInBytes;   ///< to be able to read in configurations of different version
    unsigned int version;               ///< version of the cspacerep-program
} programMetaData;



/**
 * @brief Configuration of the program.
 *        Contains such info, as when to study and max notes to study.
 * 
 */
typedef struct programConfiguration
{
    programMetaData metaData;

    unsigned int nextNoteId;
    unsigned int timeLastStarted;
    unsigned int offsetInDayToStudy;    ///< offset in seconds in day when study period should be (I.E updates)
    unsigned int numOfNotes;            ///< total number of notes
    unsigned int numOfDecks;            ///< total number of decks
    unsigned int noteSizeInBytes;       ///< needed to read in notes correctly 
    unsigned int deckSizeInBytes;       ///< needed to read in decks correctly
    unsigned int reserved;              ///< to be used by future extensions
} programConfiguration;


namespace CSPACEREP
{
    class Config
    {
        private:
            programConfiguration    data;
            std::string             configFileName;

        public:

            Config();

            void setFileName(std::string fileName);

            void load();

            void save();


            //GETTERS
            unsigned int getNextNoteId();
            

            //SETTERS - to validate input
            void setNextNoteId();
    };
}


#endif