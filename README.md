# Astro-Blaster
A simple version of the game Astro-Blaster. This was turned in as a project assignment for a class. The sound effects, graphic pictures, and SpriteManager.h were all provided.

## Dependencies
* XQuartz
* GLUT

## Usage
Once you run the program, 

## Class Descriptions
### MyHash
MyHash is an implementation of a hash table. The rules for this project made it so no mappings were allowed. Therefore, in order to reach the desired efficiency, an implementation of a hash table was necessary.

### Decrypter
The decrypter decodes the cipher by recursively checking which mapping works for all characters in the cipher. This is the class where the decoded words are returned.

### Tokenizer
The Tokenizer splits the starting cipher string into separate words and stores them into a vector.

### Translator
The translator uses a linked list that acts as a stack to store current mappings. Mappings are relating each letter in the alphabet to another letter. This is what is used to see how many matches there are in a mapping and to ultimately decode the cipher.

### WordList
The WordList is the class that stores the allowed words. The allowed words are loaded from the enigmix.txt file that has plenty of English dictionary words.

## Credit
The word list text file is found from http://www.gwicks.net/dictionaries.htm

