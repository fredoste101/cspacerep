COMPILER=g++ -std=c++11

COMPILER_FLAGS=

NCURSES_LINK=-l ncurses

TMP_BUILD=build/tmp

WINDOW_MANAGER_DIR=lib/cpplibrary/tui/windowManager

POPUP_DIR=lib/cpplibrary/tui/popup


debug : COMPILER_FLAGS=-g

debug : all


all : tmpBuild src/main.cpp memAlgo deck tui config note
	$(COMPILER) $(COMPILER_FLAGS) src/main.cpp $(TMP_BUILD)/*.o $(TMP_BUILD)/*.a $(NCURSES_LINK) -o build/cspacerep -I $(WINDOW_MANAGER_DIR) -I $(POPUP_DIR)


tmpBuild :
	if [ -d $(TMP_BUILD) ]; then echo using $(TMP_BUILD); else mkdir build/tmp; fi


test : ./test/test.c src/study/memAlgo.c
	$(COMPILER) $(COMPILER_FLAGS) -I ./src/study -o ./test/build/test.out ./test/test.c src/study/memAlgo.c 


utils : src/utils/utils.cpp src/utils/utils.h
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $(TMP_BUILD)/utils.o 


config : src/config/config.cpp src/config/config.h
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $(TMP_BUILD)/config.o


tui : src/tui/tui.cpp windowManager popup src/tui/tuiConstants.h src/tui/tui.h
	$(COMPILER) $(COMPILER_FLAGS) -c $< $(NCURSES_LINK) -o $(TMP_BUILD)/tui.o -I $(WINDOW_MANAGER_DIR) -I $(POPUP_DIR)


windowManager : lib/cpplibrary/tui/windowManager/windowManager.cpp lib/cpplibrary/tui/windowManager/windowManager.h 
	cd $(WINDOW_MANAGER_DIR) && make && cd - && cp $(WINDOW_MANAGER_DIR)/windowManager.a $(TMP_BUILD)


popup : lib/cpplibrary/tui/popup/popup.cpp lib/cpplibrary/tui/popup/popup.h 
	cd $(POPUP_DIR) && make && cd - && cp $(POPUP_DIR)/popup.a $(TMP_BUILD)


deck : src/deck/deckContainer.cpp src/deck/deckContainer.h utils
	$(COMPILER) $(COMPILER_FLAGS) $< -c -o $(TMP_BUILD)/deck.o


note : src/note/noteContainer.cpp src/note/noteContainer.h utils
	$(COMPILER) $(COMPILER_FLAGS) $< -c -o $(TMP_BUILD)/note.o

configStorage : src/storage/configStorage.c
	$(COMPILER) -c $< -o $(TMP_BUILD)/configStorage.o


memAlgo : src/study/memAlgo.c src/study/memAlgo.h
	$(COMPILER) -c $< -o $(TMP_BUILD)/memAlgo.o
