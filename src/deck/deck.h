#include <string>


typedef struct deck deck;

typedef struct deck
{
    unsigned int        size;                   ///< if struct will change between versions
    unsigned int        id;                     ///< unique ID of this deck. Used to link notes to given deck
    std::string*        name;                   ///< name of the deck
    deck*               parentP;                ///< parent deck
    std::vector<deck*>  childList;              ///< list of child decks
    unsigned int        numOfNotes;             ///< number of notes directly in this deck
    unsigned int        numOfNotesAllChildren;  ///< total number of notes in all children
    unsigned int        reserved;               ///< reserved for future use
} deck;