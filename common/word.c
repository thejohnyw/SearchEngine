/**
 * John Wang, CS50 Lab 5, May 2023
 * word.c, implements header file that contains `normalize_word` method
 * see word.h for details
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

// normalize_word - see word.h for details
char
*normalize_word(char* word)
{
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        word[i] = tolower(word[i]); // tolower from ctype
    }
    return word;
}