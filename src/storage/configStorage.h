#pragma once
#include "../config/config.h"
#include <stdio.h>
#include <stdlib.h>
#include "storageConstants.h"


int createProgramConfigFile(programConfiguration* configP);
int readProgramConfigFile(programConfiguration* configP);
int saveProgrammingConfigFile(programConfiguration* configP);
