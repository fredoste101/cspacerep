COMPILER=g++

COMPILER_FLAGS=

NCURSES_LINK=-l ncurses

TMP_BUILD=build/tmp


debug : COMPILER_FLAGS=-g

debug : all


all : tmpBuild src/main.cpp memAlgo deck tui config
	$(COMPILER) $(COMPILER_FLAGS) src/main.cpp src/note/note.c $(TMP_BUILD)/* $(NCURSES_LINK) -o build/cspacerep


tmpBuild :
	if [ -d $(TMP_BUILD) ]; then echo using $(TMP_BUILD); else mkdir build/tmp; fi


test : ./test/test.c src/study/memAlgo.c
	$(COMPILER) $(COMPILER_FLAGS) -I ./src/study -o ./test/build/test.out ./test/test.c src/study/memAlgo.c 



config : src/config/config.cpp src/config/config.h
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $(TMP_BUILD)/config.o


tui : src/tui/tui.cpp windowManager src/tui/tuiConstants.h src/tui/tui.h
	$(COMPILER) $(COMPILER_FLAGS) -c $< $(NCURSES_LINK) -o $(TMP_BUILD)/tui.o


windowManager : src/tui/windowManager.cpp src/tui/windowManager.h
	$(COMPILER) $(COMPILER_FLAGS) -c src/tui/windowManager.cpp $(NCURSES_LINK) -o $(TMP_BUILD)/windowManager.o 


deck : src/deck/deck.c src/deck/deck.h
	$(COMPILER) $(COMPILER_FLAGS) $< -c -o $(TMP_BUILD)/deck.o


configStorage : src/storage/configStorage.c
	$(COMPILER) -c $< -o $(TMP_BUILD)/configStorage.o


memAlgo : src/study/memAlgo.c src/study/memAlgo.h
	$(COMPILER) -c $< -o $(TMP_BUILD)/memAlgo.o
