/**
 * John Wang, CS50 Lab 5, May 2023
 * word.h, header file that contains `normalize_word` method
 * implemented in word.c
*/

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**************** normalize_word(char* word) ****************/
/* normalizes inputted word
 *
 * Caller provides:
 *   a pointer to a word
 * We return:
 *    the word lowercase
 */

char* normalize_word(char* word);

#endif