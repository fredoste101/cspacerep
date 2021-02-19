COMPILER=gcc

COMPILER_FLAGS=

NCURSES_LINK=-l ncurses


debug : COMPILER_FLAGS=-g

debug : all


all : src/main.c memAlgo storage
	$(COMPILER) $(COMPILER_FLAGS) -o build/main.o build/memAlgo.o build/notesStorage.o build/configStorage.o src/main.c src/study/study.c src/notes/note.c  src/tui/tuiUtils.c $(NCURSES_LINK)



test : ./test/test.c src/study/memAlgo.c
	$(COMPILER) $(COMPILER_FLAGS) -I ./src/study -o ./test/build/test.out ./test/test.c src/study/memAlgo.c 


storage : notesStorage configStorage src/storage/storageConstants.h


notesStorage : src/storage/notesStorage.c
	gcc -c src/storage/notesStorage.c -o build/notesStorage.o


configStorage : src/storage/configStorage.c
	gcc -c src/storage/configStorage.c -o build/configStorage.o


memAlgo : src/study/memAlgo.c src/study/memAlgo.h
	gcc -c src/study/memAlgo.c -o build/memAlgo.o