# CS280: PLSL Parser & Interpreter
The following is an interpreter I developed for my CS280 (Programming Language Concepts) class.

This project is built on two other components that I had developed earlier in the class.

1. A lexical analyzer
2. A recursive-descent parser for a Pascal-Like Simple Language.

The grammar rules of the PLSL was described in EBNF notations.

# Interpreter Functions

Using previous components (lexical analyzer and parser), the interpreter is responsible for:

1. Building information of variables types in a map container for all the defined variables.
2. Evaluating expressions and determining their values and types.
3. Using a map container that keeps a record of the defined variables in the parsed program.

4. Printing the results of an unsuccessful parsing and interpreting by the parser/interpreter functions, as well as error messages that might be detected by the lexical analyzer.

# PLSL Description

The PLSL consists of three main parts in the respective order.
1. <em>Program Declaration</em>
2. <em>Variable Declaration</em>
3. <em>Program Body</em>

## Program Declaration
The Program Declaration block consists of <code>PROGRAM</code> token followed by an identifier in the beginning of a program. 
<code>PROGRAM circle;</code>

## Variable Declaration
The Variable Declaration block is where the variables that will be used in the program are declared. 
The block is start with the <code>VAR</code> token then followed by variable declarations.
<pre><code>VAR
	r, a, p, b : REAL;
	i, j, sum : INTEGER;
	str1, str2: STRING; </code></pre>
### Types
PLSL has three different types.
1. INTEGER
2. REAL
3. STRING
## Program Body
The beginning of the Program Body is defined by the <code>BEGIN</code> token and the end of the body is defined with the <code>END</code> token.
<pre><code>BEGIN
	i := 8;
	p := 6;
	a := 0;
	sum := 0;
	str1 := 'Hello World!';
	j := -3;
	IF ( p < 5) THEN
		a := (3.14) + j * - p
	ELSE
		IF (p > 0 ) THEN 
			str1 := 'Goodbye!';
	(*Display the results*)
	WRITELN ('The output results are: ' , p, ' ', str1, ' ', a, ' ', i);
END
</pre></code>
