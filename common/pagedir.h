/*
 * John Wang, CS50, Lab 4, pagedir.h
 * details on the the methods to initialize and save a page directory
 * contains methods: 
 * bool pagedir_init(const char* pageDirectory);
 * void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
 * bool pagedir_validate(const char* pageDirectory);
 * webpage_t* pagedir_getPage(const char* pageDirectory, const int docID);
*/
#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

// functions
/**************** pagedir_init ****************/
/* Initialize crawler file in inputted directory
 *
 * Caller provides:
 *   a valid pointer to a page directory
 * We return:
 *   true if correctly initialized
 *   false if any errors
 */
bool pagedir_init(const char* pageDirectory);


/**************** pagedir_save ****************/
/* Saves webpage to inputted directory with inputted id
 *
 * Caller provides:
 *   a valid pointer to a webpage, a valid pointer to a page directory, and an id

 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);



/**************** pagedir_validate ****************/
/* checks if inputed directory is a crawler-produced directory
 * Basically a copy of pagedir_init except this checks for file reading while init writes
 *
 * Caller provides:
 *   a pointer to a page directory
 * We return:
 *    True if it is a crawler-produced directory
 *    False if not
 */
bool pagedir_validate(const char* pageDirectory);

/**************** pagedir_getPage ****************/
/* gets webpage from docID in page directory
 *
 * Caller provides:
 *   a pointer to a page directory and docID
 * We return:
 *    the webpage associated with the docID in the pageDirectory
 */
webpage_t* pagedir_getPage(const char* pageDirectory, const int docID);

#endif