#!/bin/bash


#DIRECTORY TO STORE FILES USED BY CSPACEREP
CSPACE_REP_STORAGE_DIR="/var/cspacerep"



if [ -d $CSPACE_REP_STORAGE_DIR ]; then
    echo "working dir ${CSPACE_REP_STORAGE_DIR} exists"
else
    mkdir $CSPACE_REP_STORAGE_DIR
fi

touch ./src/storage/directory.h

echo "//GENERATED FILE" > ./src/storage/directory.h
echo "#define STORAGE_DIR \"${CSPACE_REP_STORAGE_DIR}\"" >> ./src/storage/directory.h

echo "*** INSTALLATION COMPLETE ***"