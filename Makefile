COMPILER=gcc

COMPILER_FLAGS=


debug : COMPILER_FLAGS=-g

debug : all


all : main.c 
	$(COMPILER) $(COMPILER_FLAGS) -o build/main.o main.c