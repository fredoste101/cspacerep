#pragma once

#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>

std::vector<std::string*>* binaryFileStringReader(std::fstream* fileP, 
                                                  unsigned int numOfStringsToRead);
