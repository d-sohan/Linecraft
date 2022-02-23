CC = g++
CFLAGS = -Wall
 
 
linecraftc: linecraftc.cpp include/tokens.cpp include/utilities.cpp include/tokens.h include/utilities.h
	$(CC) $(CFLAGS) -o linecraftc linecraftc.cpp include/tokens.cpp include/utilities.cpp
 