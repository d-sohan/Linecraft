# Linecraft
Linecraft is a custom language used to demonstrate the workings of lexer and parser during compilation.

Currently, we are working on the lexer.

## Compilation Instructions
To compile the Linecraft compiler use:  
```g++ linecraftc.cpp include/tokens.cpp include/utilities.cpp -o linecraftc```

or using make:  
```make```

To run on a file (e.g. ```test.lcft```): 
```./linecraftc test.lcft```


## File Descriptions:  
```linecraftc.cpp``` is the main file which is used to extract tokens from a file.  
It uses utility functions defined in ```include/utilities.cpp``` and "DFA functions", which implement the dfas for token extraction, defined in ```include/tokens.cpp```



Please read the notes given in the comments of following files carefully before adding new functions!

### Steps to add new tokens :
1. Add the DFA function definitions in ```tokens.cpp```
2. Add the DFA function declarations in ```tokens.h```
3. Add the DFA function name (without parenthesis) in ```linecraftc.cpp```






