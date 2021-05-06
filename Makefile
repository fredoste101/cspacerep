COMPILER=g++

COMPILER_FLAGS=

NCURSES_LINK=-l ncurses


debug : COMPILER_FLAGS=-g

debug : all


all : src/main.cpp memAlgo storage deck tui
	$(COMPILER) $(COMPILER_FLAGS) build/memAlgo.o build/notesStorage.o build/configStorage.o build/windowManager.o build/tui.o src/main.cpp src/study/study.c src/notes/note.c $(NCURSES_LINK) -o build/main.o


test : ./test/test.c src/study/memAlgo.c
	$(COMPILER) $(COMPILER_FLAGS) -I ./src/study -o ./test/build/test.out ./test/test.c src/study/memAlgo.c 


storage : notesStorage configStorage src/storage/storageConstants.h


tui : windowManager src/tui/tuiConstants.h src/tui/tuiUtils.cpp src/tui/tuiUtils.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/tui/tuiUtils.cpp $(NCURSES_LINK) -o build/tui.o


windowManager : src/tui/windowManager.cpp src/tui/windowManager.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/tui/windowManager.cpp $(NCURSES_LINK) -o build/windowManager.o 


deck : src/deck/deck.c src/deck/deck.h
	$(COMPILER) $(COMPILER_FLAGS) -c $^ -o build/deck.o


notesStorage : src/storage/notesStorage.c
	$(COMPILER) -c src/storage/notesStorage.c -o build/notesStorage.o


configStorage : src/storage/configStorage.c
	$(COMPILER) -c src/storage/configStorage.c -o build/configStorage.o


memAlgo : src/study/memAlgo.c src/study/memAlgo.h
	$(COMPILER) -c src/study/memAlgo.c -o build/memAlgo.o
