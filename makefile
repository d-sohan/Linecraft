CC = g++
CFLAGS = -Wall
 
 
linecraftc: main.cpp include/tokens.cpp include/utilities.cpp parser.cpp include/tokens.h include/utilities.h parser.h
	$(CC) $(CFLAGS) -o linecraftc main.cpp include/tokens.cpp include/utilities.cpp parser.cpp
 