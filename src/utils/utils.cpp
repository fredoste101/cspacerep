#include "utils.h"

#define BUFFER_NUM_OF_CHARS 100

/**
 * @brief Reads in from file a list of strings one after the other, separeted by '\0' (classic c-string)
 *        A maximum of numOfStringsToRead is read. Then file pointer is set to end of last string.
 * 
 * @param fileP 
 * @param numOfStringsToRead 
 * @return std::vector<std::string*>* 
 */
std::vector<std::string*>* binaryFileStringReader(std::fstream* fileP, 
                                                  unsigned int numOfStringsToRead)
{

    std::vector<std::string*>* stringListP = new std::vector<std::string*>;

    char buf[BUFFER_NUM_OF_CHARS];

    unsigned int stringIndex = 0;


    std::string currentString = "";

    std::streamsize numOfBytesRead;

    do
    {

        memset(buf, 0, sizeof(char) * BUFFER_NUM_OF_CHARS);

        fileP->read(buf, sizeof(char) * BUFFER_NUM_OF_CHARS);

        numOfBytesRead = fileP->gcount();

        fprintf(stderr, "numOfBytesRead=%lu", numOfBytesRead);

        for(unsigned int i = 0; i < numOfBytesRead; i++)
        {
            if(buf[i] == '\0')
            {

                std::string* tmpStringP = new std::string();

                tmpStringP->assign(currentString);

                stringIndex++;

                fprintf(stderr, "\nread %s\n", tmpStringP->c_str());

                stringListP->push_back(tmpStringP);

                if(stringIndex >= numOfStringsToRead)
                {
                    fileP->seekg(-(numOfBytesRead - i), std::ios_base::cur);
                    fileP->seekp(-(numOfBytesRead - i), std::ios_base::cur);
                    break;
                }

                currentString = "";
            }
            else
            {
                currentString += buf[i];
            }
        }
    } 
    while (numOfBytesRead == (sizeof(char) * BUFFER_NUM_OF_CHARS) && stringIndex < numOfStringsToRead);

    fprintf(stderr, "\n%s\n", currentString.c_str());

    return stringListP;
}