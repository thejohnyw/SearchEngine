/*
* John Wang, CS50, TSE Lab 6
* querier.c
* compiled program queries files
* Usage: ./querier pageDirectory indexFilename
* where pageDirectory is data directory made by crawler
* And, indexFilename is output file made by indexer
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/counters.h"
int fileno(FILE* stream); // from stdio


// structs

// holding 2 counters
struct twoCounters {
    counters_t* ctr1;
    counters_t* ctr2;
};


// holds topScore and associating docID
struct topScore {
    int score;
    int docID;
};

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
    // indexFilename cannot be read
    FILE *fp = fopen(argv[2], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot read from indexFilename\n");
        exit(1);
    }
    fclose(fp);
}

// prompts user for query
static void
prompt(void)
{
    if (isatty(fileno(stdin))) {
        printf("Query? \n");
    }

}

// return smaller int; used for getting smaller count
static int
min(const int a, const int b)
{
    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}

// ensures that users' query is valid ('and' and 'or' are where they're supposed to be)
static bool
validQuery(char** words, int count)
{
    if (count < 0) {
        fprintf(stderr, "count of words cannot be < 0\n");
        return false;
    }
    // check that 'and' or 'or' isnt first
    if ((strcmp(words[0], "and") == 0) || (strcmp(words[0], "or") == 0)) {
        fprintf(stderr, "Error: '%s' cannot be first\n", words[0]);
        return false;
    }
    // check that 'and' or 'or' isnt last
    if ((strcmp(words[count-1], "and") == 0) || (strcmp(words[count-1], "or") == 0)) {
        fprintf(stderr, "Error: '%s' cannot be last\n", words[0]);
        return false;
    }
    // start at 1 and end at count-1 since checked 0th and last index already
    for (int i = 1; i < count - 1; i++) {
        if ((strcmp(words[i], "and") == 0) || (strcmp(words[i], "or") == 0)) {
            if ((strcmp(words[i+1], "and") == 0) || (strcmp(words[i+1], "or") == 0)) {
                fprintf(stderr, "Error: '%s' and '%s' cannot be next to each other\n", words[i], words[i+1]);
                return false;
            }
        }
    }
    return true;

}


// returns # of words in input; splits text into words and stores them in words
static int
parseQuery(char** words, char* line)
{
    int count = 0;
    char* start = line;
    char* end = line;
    bool inWord = false;
    while (*end != '\0') {
        if (isspace(*end)) { // if isspace: mark end & store word
            if (inWord) {
                *end = '\0';
                words[count] = normalize_word(start);
                count++;
                inWord = false;
            }
            
        }
        else { // if alpha or number
            if (!inWord) {
                inWord = true;
                start = end;
            }
        }
        end++;
    }
    // increment if last is inWord
    if (inWord) {
        words[count] = normalize_word(start);
        count++;

    }
    return count;
}

// union functions
// helper for iterator to update count
static void
unionHelper(void* arg, const int key, const int count)
{
    counters_t* ctr = arg;
    int ctrCount = counters_get(ctr, key);
    counters_set(ctr, key, count + ctrCount);
}

// union function: used for 'or' calls: get total matches
static void
unionCtr(counters_t* allWords, counters_t* currWord)
{
    counters_iterate(currWord, allWords, unionHelper);
}

// intersect functions

// helper for iterator to update count for intersect
static void
intersectHelper(void* arg, const int key, const int count)
{
    struct twoCounters* both = arg;
    counters_set(both->ctr1, key, min(count, counters_get(both->ctr2, key)));
}


// intersect function: used for 'and' calls: get intersecting matches
static void
intersect(counters_t* allWords, counters_t* currWord)
{
    struct twoCounters both = {allWords, currWord};
    counters_iterate(allWords, &both, intersectHelper);
}



// tries to find matches based on query; returns counter of words
static counters_t* 
matchingQuery(char** words, int maxCount, index_t* index)
{
    counters_t* finalCtr = counters_new();
    counters_t* currCtr = NULL;
    for (int i = 0; i < maxCount; i++) {
        if (strcmp(words[i], "or") == 0) {
            unionCtr(finalCtr, currCtr);
            mem_free(currCtr);
            currCtr = NULL;
            continue;
        }
        else if (strcmp(words[i], "and") == 0) {
            continue;
        }
        else {
            counters_t* wordCt = index_find(index, words[i]);
            if (currCtr == NULL) {
                currCtr = counters_new();
                unionCtr(currCtr, wordCt); // or
            }
            else { // and
                intersect(currCtr, wordCt);
            }

        }
    }
    unionCtr(finalCtr, currCtr);
    counters_delete(currCtr);
    return finalCtr;
}

// updates topScore struct in iterations to have the doc with best score
static void
updateScore(void* arg, const int key, const int count)
{
    struct topScore* top = arg;
    if (count > top->score) {
        top->score = count;
        top->docID = key;
    }
}

// counts # of docs in iterations
static void
countDocs(void* arg, const int key, const int count)
{
    int* docCount = arg;
    if (count > 0) {
        (*docCount)++;
    }
}

// reads URL from file and prints the docs
static void
printMatch(char* pageDirectory, const int docID, const int count)
{
    if (pageDirectory == NULL) {
        fprintf(stderr, "pageDirectory is NULL");
        exit(1);
    }
    char* fileName = mem_malloc_assert((strlen(pageDirectory) + 11), "Cannot memory assert filepath string");
    sprintf(fileName, "%s/%d", pageDirectory, docID);
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        fprintf(stderr, "cannot open file for reading");
        exit(1);
    }
    char* URL = file_readLine(fp);
    fclose(fp);
    mem_free(fileName);
    printf("score %d  doc %d: %s\n", count, docID, URL);
    mem_free(URL);
}


// Ranks counters via selection sort
static int
rankCounters(counters_t* counter, char* pageDirectory)
{
    int docCount = 0;
    counters_iterate(counter, &docCount, countDocs);
    if (docCount == 0) {
        printf("No matching docs\n");
    }
    else {
        printf("Found %d docs. Ranked results: \n", docCount);
        for (int i = 0; i < docCount; i++) {
            struct topScore* current = mem_malloc(sizeof(struct topScore));
            current->score = 0;
            current->docID = 0;
            counters_iterate(counter, (void*)current, updateScore); // get max doc
            printMatch(pageDirectory, current->docID, current->score); // printing matches
            counters_set(counter, current->docID, 0); // set curr max to 0 for descending sorting
            mem_free(current);
        }
    }
    return docCount;

}

// pieces everything together to process query and print results
static void
query(index_t* index, char* pageDirectory)
{
    prompt();

    while(!feof(stdin)) {
        char* line = file_readLine(stdin);
        if (line == NULL) {
            continue;
        }
        char* words[strlen(line)/2];
        int maxIdx = parseQuery(words, line); // check how many words
        if (maxIdx != 0) {
            printf("Your query: "); // print user query
            for (int i = 0; i < maxIdx; i++) {
                printf("%s ", words[i]);
            }
            printf("\n");
            if (validQuery(words, maxIdx)) { // do matching and ranking if valid
                counters_t* res = matchingQuery(words, maxIdx, index);
                int docCount = rankCounters(res, pageDirectory);
                if (docCount == 0) {
                    printf("--------------------------------------------------------------\n");
                    prompt();
                    continue;
                }
                counters_delete(res);
            }
            else { // prompt again because invalid query
                printf("--------------------------------------------------------------\n");
                prompt();
                continue;
            }
        }
        else { // prompt again because no words at first
            printf("--------------------------------------------------------------\n");
            prompt();
            continue;
        }
        mem_free(line);
        printf("--------------------------------------------------------------\n");
        prompt();
    }
}

// main: loads in index, runs query, and deletes index
int
main(int argc, char* argv[])
{
    parseArgs(argc, argv);
    index_t* index = index_load(argv[2]);
    if (index == NULL) {
        fprintf(stderr, "loading index failed");
        exit(3);
    }
    query(index, argv[1]);
    index_delete(index);
    return 0;
}
