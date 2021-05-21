#include "config.h"

using namespace CSPACEREP;

Config::Config()
{
    data.metaData.sizeOfConfigInBytes   = sizeof(data);
    data.metaData.version               = VERSION;

    data.deckSizeInBytes = sizeof(deck);
    data.noteSizeInBytes = sizeof(note);
}


void Config::setFileName(std::string fileName)
{
    configFileName = fileName;
}


void Config::load()
{
    std::fstream configFile(configFileName.c_str(), std::ios::binary);

    char buf[100];

    configFile.read(buf, 100);

    memcpy(&data, buf, sizeof(data));

    if(sizeof(data) == data.metaData.sizeOfConfigInBytes)
    {
        //All good
    }
    else
    {
        //Wrong version
        exit(1);
    }
    
}


void Config::save()
{

}
