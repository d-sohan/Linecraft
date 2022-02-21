# Linecraft
Linecraft is a custom language used to demonstrate the workings of lexer and parser during compilation.

Currently, we are working on the lexer.




Please read the notes given in the comments of following files carefully before adding new functions!

Steps to add new tokens :
1. Add the DFA function definitions in ```tokens.cpp```
2. Add the DFA function declarations in ```tokens.h```
3. Add the DFA function name (without parenthesis) in ```linecraftc.cpp```



To compile the Linecraft compiler ```linecraftc``` use:
```g++ linecraftc.cpp tokens.cpp include/utilities.cpp -o linecraftc```

To run on a file (e.g. ```test.lcft```):
```./linecraftc test.lcft```


