/**
 * John Wang, CS50 Lab 5, May 2023
 * index.h, header file that contains index methods
 * implemented in index.c
*/

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new(int slots) ****************/
/* make new index
 *
 * Caller provides:
 *   a number of slots
 * We return:
 *    a pointer to index structure with the inputted number of slots
 */
index_t* index_new(int slots);

/**************** index_save(index_t* index, char* path) ****************/
/* save index to directory
 *
 * Caller provides:
 *   a pointer to an index and a file path string to save into
 * We return:
 *    true if saved correctly
 *    false otherwise
 */
void index_save(index_t* index, char* path);


 /**************** index_delete(index_t* index) ****************/
/* delete the index
 *
 * Caller provides:
 *   a pointer to an index
 * We delete the index and every item in it
 */
void index_delete(index_t* index);

/**************** index_load(char* path) ****************/
/* load index from file
 *
 * Caller provides:
 *   a pointer to a filepath
 * We return:
 *    pointer to index
 */
index_t* index_load(char* path);

/**************** index_find(index_t* index, char* word) ****************/
/* get counters from index
 *
 * Caller provides:
 *   a pointer to an index and word
 * We return:
 *    pointer to counters for the word in the index
 */
counters_t* index_find(index_t* index, char* word);

 






#endif