#pragma once


#define VERSION 0


typedef struct programMetaData
{
    unsigned int sizeOfConfigInBytes;          ///< to be able to read in configurations of different version
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
    unsigned int offsetInDayToStudy;   ///< offset in seconds in day when study period should be (I.E updates)
    unsigned int numOfNotes;
    unsigned int noteSizeInBytes;       ///< needed to read in notes correctly 
    unsigned int deckSizeInBytes;       ///< needed to read in decks correctly
    unsigned int reserved;              ///< to be used by future extensions
} programConfiguration;