#include "configStorage.h"


void createProgramConfigFile(programConfiguration* configP)
{
    FILE* configFile = fopen(CONFIG_FILE, "w+");

    if(configFile == NULL)
    {
        fatalError("Could not open/create erispacerep.conf. FATAL");
    }

    size_t bytesWritten = fwrite(configP, 
                                 sizeof(*configP), 
                                 1, 
                                 configFile);

    fclose(configFile);

}

int readProgramConfigFile(programConfiguration* configP)
{
    FILE* configFile = fopen(CONFIG_FILE, "r");

    if(configFile == NULL)
    {
        return 0;
    }
    else
    {
        size_t memberRead = fread((void*)configP, 
                                  sizeof(*configP), 
                                  1, 
                                  configFile);

        if(memberRead != 1)
        {
            fclose(configFile);
            fatalError("bytesRead is not equal to config size. FATAL.");
        }

        fclose(configFile);
        return 1;
    }
}


int saveProgrammingConfigFile(programConfiguration* configP)
{
    FILE* configFile = fopen(CONFIG_FILE, "w");

    if(configFile == NULL)
    {
        fatalError("error opening file with w. FATAL");
    }

    fwrite(configP, 
           sizeof(*configP), 
           1, 
           configFile);

    fclose(configFile);
}
