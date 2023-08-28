/*
 * John Wang, CS50, pagedir.c
 * implementation of pagedir.h
 * see pagedir.h for specifics
 * contains methods: 
 * bool pagedir_init(const char* pageDirectory);
 * void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
 * bool pagedir_validate(const char* pageDirectory);
 * webpage_t* pagedir_getPage(const char* pageDirectory, const int docID);
*/
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Global functions */
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_getPage(const char* pageDirectory, const int docID);

// see pagedir.h for details
bool
pagedir_init(const char* pageDirectory)
{
    // construct the pathname for the .crawler file in that directory
    char *newPath = mem_malloc(strlen(pageDirectory) + strlen("./crawler") + 1);
    // copy/concat path
    strcpy(newPath, pageDirectory);
    strcat(newPath, "/.crawler");
    FILE *fp;
    // open for writing & return false on error
    fp = fopen(newPath, "w");
    if (fp == NULL) {
        mem_free(newPath);
        return false;
    }
    // close and return true
    fclose(fp);
    mem_free(newPath);
    return true;
}

// see pagedir.h for details
void
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
    char* path = mem_malloc(strlen(pageDirectory) + 10);
    // constructing/opening path
    sprintf(path, "%s/%d", pageDirectory, docID);
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error writing file");
        exit(1);
    }
    // print these to file
    fprintf(fp, "%s\n", webpage_getURL(page));
    fprintf(fp, "%d\n", webpage_getDepth(page));
    fprintf(fp, "%s\n", webpage_getHTML(page));
    fclose(fp);
    mem_free(path);
}

// see pagedir.h for details
bool
pagedir_validate(const char* pageDirectory)
{
    // check if hidden file .crawler in directory
    char *direct = mem_malloc(strlen(pageDirectory) + strlen("./crawler") + 1);
    // constructing/opening path
    sprintf(direct, "%s/.crawler", pageDirectory);
    FILE *fp = fopen(direct, "r");
    // if .crawler isnt in directory, then it FALSE
    if (fp == NULL) {
        mem_free(direct);
        return false;
    }
    // else: return true
    fclose(fp);
    mem_free(direct);
    return true;

}
// see pagedir.h for details
webpage_t*
pagedir_getPage(const char* pageDirectory, const int docID)
{
    // assuming 11 is max digits
    char* path = malloc(strlen(pageDirectory) + 11);
    sprintf(path, "%s/%d", pageDirectory, docID);
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        mem_free(path);
        return NULL;
    }
    // get page from file
    char* url = file_readLine(fp);
    char* depth = file_readLine(fp);
    int dep = atoi(depth);
    char* html = file_readFile(fp);
    webpage_t* page = webpage_new(url, dep, html);
    mem_free(depth);
    mem_free(path);
    fclose(fp);
    return page;

}
