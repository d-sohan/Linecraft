# Linecraft
Linecraft is a custom language used to demonstrate the workings of lexer and parser during compilation.

Currently, we are working on the lexer.


To add new tokens (placeholders given in each file where you should add):
1. Add DFA function definitions in ```tokens.cpp```
2. Add DFA functino declarations in ```tokens.h```
3. Add the function name (without parenthesis) in ```lexer.cpp```



To compile use:
```g++ lexer.cpp include/utilities.cpp tokens.cpp -o lexer```

To run:
```./lexer test.cpp```


