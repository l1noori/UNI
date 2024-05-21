Opdracht 3 Datastructuren
09/02/2024
Author: Noah van Laarhoven

This program has the following functions:
* exp <expression>
Read the expression and creates a corresponding automaton, saving it using vectors. 
This vector consists of structs, which each have the State number, the necessary character, and up to two following states

Accepted symbols are:
a-z lowercase lettters
 |  choose
 *  repeat
( ) brackets

* dot <filename>
Saves the given automaton using the 'exp' function to the given file, in DOT notation

* mat <string>
Checks whether the given string is accepted by the saved automaton giving either 'Match' or 'No match'. '$' can be used to denote an empty string.

EXTRA:

* tab
Gives current automaton in table form.