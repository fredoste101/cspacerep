#include "configStorage.h"





int createProgramConfigFile(programConfiguration* configP)
{
    FILE* configFile = fopen(CONFIG_FILE, "w+");

    if(configFile == NULL)
    {
	    return -1;
    }

    size_t bytesWritten = fwrite(configP, 
                                 sizeof(*configP), 
                                 1, 
                                 configFile);

    fclose(configFile);
    return 0;

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
	    return -1;
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
	return -1;
    }

    fwrite(configP, 
           sizeof(*configP), 
           1, 
           configFile);

    fclose(configFile);
    return 0;
}
