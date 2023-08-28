/**
 * indexer.c
 * John Wang, CS50 Lab 5
 * May 1, 2023
 * compiled program indexes files in a directory
 * Input: in bash, a `pageDirectory`, the pathname of a directory produced by the Crawler
 * And, a `indexFilename`, the pathname of file where index should be written
 * Output: Program creates the indexFilename file or overwrites it
 * usage: ./indexer pageDirectory indexFilename
 * exit error of 1 for any errors (wrong #, invalid directory/file writing)
 * exit 0 if runs correctly
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"



// parseArgs: validate proper input of user args
static void
parseArgs(int argc, char* argv[])
{
    // incorrect # of args
    if (argc != 3) {
        fprintf(stderr, "Usage error; Wrong # of args; proper use: %s [pageDirectory] [indexFilename]\n", argv[0]);
        exit(1);
    }

    // invalid pageDirectory
    if (!pagedir_validate(argv[1])) {
        fprintf(stderr, "Error: pageDirectory isn't one created by crawler\n");
        exit(1);
    }
    // indexFilename cannot be written to
    FILE *fp = fopen(argv[2], "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot write into indexFilename\n");
        exit(1);
    }
    fclose(fp);
}

// indexPage: scans a webpage document to add its words to the index
static void
indexPage(webpage_t* page, index_t* index, int docID)
{
    char* currWord;
    int pos = 0;
    while ((currWord = webpage_getNextWord(page, &pos)) != NULL) {
        // ignore lengths < 3
        if (strlen(currWord) < 3) {
            mem_free(currWord);
            continue;
        }
        // normalize word
        currWord = normalize_word(currWord);
        // look up word in index
        // increment count
        if (hashtable_find(index, currWord) != NULL) {
            counters_t* counters = hashtable_find(index, currWord);
            counters_add(counters, docID);
        }
        // add if need
        else {
            counters_t* newCounters = counters_new();
            counters_add(newCounters, docID);
            hashtable_insert(index, currWord, newCounters);
        }
        mem_free(currWord);
        
    }
}

// indexBuild: builds/returns an index from webpage files it finds in the pageDirectory
static index_t*
indexBuild(char* pageDirectory)
{
    /**
      creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage
    */
    index_t* idx = index_new(600);
    int docID = 1;
    webpage_t* currPage;
    while ((currPage = pagedir_getPage(pageDirectory, docID)) != NULL) {
        indexPage(currPage, idx, docID);
        webpage_delete(currPage);
        docID++;
    }
    return idx;
    
}

// main: checks arguments, builds index, save it, then delete it; exit 0 if fine
int
main(int argc, char* argv[])
{
    parseArgs(argc, argv);
    index_t* index = indexBuild(argv[1]);
    index_save(index, argv[2]);
    index_delete(index);
    return 0;

}

