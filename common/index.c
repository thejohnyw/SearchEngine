/**
 * John Wang, CS50 Lab 5, May 2023
 * index.c, implemention of methods in index.h
 * see index.h for details
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "index.h"

typedef hashtable_t index_t;

// local functions
// helper to print each word for counter
static void
countHelper(void* arg, const int docID, const int count)
{
    fprintf((FILE*) arg, "%d %d ", docID, count);
}

// helper to print each word for index
static void
printHelper(void* arg, const char* word, void* counter)
{
    fprintf((FILE*) arg, "%s ", word);
    counters_iterate((counters_t*) counter, (FILE*) arg, countHelper);
    fprintf((FILE*) arg, "\n");
}

// helper to delete counters in index
static void
deleteCounter(void* counter)
{
    counters_t* ctr = counter;
    counters_delete(ctr);
}

// global functions
// see index.h for details
index_t*
index_new(int slots)
{
    return hashtable_new(slots);
}

// see index.h for details
void
index_save(index_t* index, char* path)
{
    FILE* fp = fopen(path, "w");
    if (fp == NULL) {
        fprintf(stderr, "Cannot write into file %s\n", path);
        exit(2);
    }
    hashtable_iterate(index, fp, printHelper);
    fclose(fp);
}

// see index.h for details
void
index_delete(index_t* index)
{
    hashtable_delete(index, deleteCounter);
}

// see index.h for details
index_t*
index_load(char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        return NULL;
    }
    index_t* idx = index_new(600); // between 200-1000 slots
    char* currWord;
    while ((currWord = file_readWord(fp)) != NULL) {
        counters_t* ctrs = counters_new();
        int count;
        int docID;
        while (fscanf(fp, "%d %d", &docID, &count) == 2) {
            counters_set(ctrs, docID, count); // get counters data
        }
        hashtable_insert(idx, currWord, ctrs); // set into our idx
        mem_free(currWord);
    }
    mem_free(currWord); // free NULL word
    fclose(fp);
    return idx;
}
// see index.h for details
counters_t*
index_find(index_t* index, char* word)
{
    return hashtable_find(index, word);
}