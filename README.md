# CS280: PLSL Parser & Interpreter
An interpreter I developed for my CS280 (Programming Language Concepts) class. 

---------------------------
This project is built on two other components that I had developed earlier in the class.
1. A lexical analyzer
2. A recursive-descent parser for a Pascal-Like Simple Language. 
   The grammar rules of the PLSL was described in EBNF notations.
---------------------------

# Interpreter Functions
Using previous components (lexical analyzer and parser), the interpreter is responsible for:
1. Building information of variables types in a map container for all the
defined variables.
2. Evaluating expressions and determining their values and types.
3. Using a map container that keeps a record of the defined variables in the
parsed program.
4. Printing the results of an unsuccessful parsing and interpreting by the parser/interpreter functions, as well as error
messages that might be detected by the lexical analyzer.
