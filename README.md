# AES_CPP_Implementation

COMPILE WITH:
g++ -o aes aes.cpp

RUN WITH:
./aes

Changelog 1.0:
-Brought git repository up to date with current state of project.

To-do List:
-Spellcheck comments
-Implement error checking
-Research and add license and .gitignore (first time github user, my apologies)
-Figure out how to reconvert converted text into plain text.



DESCRIPTION:
This project was originally a school project designed to demonstrate understanding
of how AES encryption works. The program first prompts the user for the name of the 
plaintext file, key file, and output file. Using this information the program
opens the relevant files and encrypts the text, saving it in the output file.
 The encryption steps are as follows:

-Preprocessing: the program prepares the plaintext file by removing punctuation and
spaces

-Substitution: the program performs polyalphabetic substitution using a vigenere
cipher

-Padding: If the string does not form 4x4 blocks, A's are inserted until that
requirement is med

-Shift Rows: Shift the rows in each block to the left zero, one, two, and three times
from top to bottom

-Parity Bit: Convert each character to number and use the most significant bit
as a parity bit

-Mix Columns: Diffuses data using Rijndael's Galois field.


Criticism, comments and suggestions are welcome. However, this project is intended
to be a showcase of my own technical ability, as such I will refuse any push requests
that include anything beyond additional comments.
