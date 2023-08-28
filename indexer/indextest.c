/**
 * indextest.c
 * John Wang, CS50 Lab 5
 * May 1, 2023
 * Usage: ./indextest oldIndexFilename newIndexFilename
 * where oldIndexFilename is the name of a file produced by the indexer, and
 * where newIndexFilename is the name of a file into which the index should be written.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/index.h"

// parseArgs: make sure arguments are valid
static void
parseArgs(int argc, char* argv[])
{
    // incorrect # of args
    if (argc != 3) {
        fprintf(stderr, "Incorrect # of args; Usage: ./indextest oldIndexFilename newIndexFilename");
        exit(1);
    }
    
    // oldIndexFilename cannot be read
    FILE *fp1 = fopen(argv[1], "r");
    if (fp1 == NULL) {
        fprintf(stderr, "Error: cannot read oldIndexFilename\n");
        exit(1);
    }
    fclose(fp1);

    // newIndexFilename cannot be written to
    FILE *fp = fopen(argv[2], "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot write into newIndexFilename\n");
        exit(1);
    }
    fclose(fp);
}

// main: check args and save index to new file
int
main(int argc, char* argv[])
{
    parseArgs(argc, argv);
    index_t* idx = index_load(argv[1]);
    if (idx == NULL) {
        fprintf(stderr, "Error: cannot load oldIndexFile\n");
        exit(1);
    }
    // save to new index file
    index_save(idx, argv[2]);
    index_delete(idx);
    return 0;

}
 