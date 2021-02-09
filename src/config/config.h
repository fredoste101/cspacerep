#pragma once


typedef struct programConfiguration
{
    unsigned int nextNoteId;
    unsigned int timeLastStarted;
    unsigned int offsetInDayToStudy;   ///< offset in seconds in day when study period should be (I.E updates)
    unsigned int numOfNotes;
} programConfiguration;