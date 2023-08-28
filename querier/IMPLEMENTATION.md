# CS50 TSE Querier Implementation details for Lab 6
## John Wang (thejohnyw), May 2023


## Data structures 

We make 2 structures. 
One to hold 2 counters
Another to hold the topScore and its associating docID

## Control flow

The querier is implemented in one file `querier.c` with 16 functions

### main

The `main` function simply calls `parseArgs` and `query` and `index_delete` (from index.c); `query` starts the stdin and other processes and prints matches, then exits zero.

### parseArgs

Given arguments from the command line, make sure they're valid

* if any trouble is found, print an error to stderr and exit non-zero.

### validQuery

ensures that users' query is valid ('and' and 'or' are where they're supposed to be)
Pseudocode:

    if count of words < 0
      error, return false
    if starts with 'and'
      error, return false
    if starts with 'or'
      error, return false
    if 'and' and 'or' next to each other
      error, return false
    return true if none of above

### parseQuery

    returns # of words in input; splits text into words and stores them in words

Pseudocode:

    loop over line
      if at space in line
        if was at word
          mark as end of word
          normalize the word and store it in array
          increment count of words
      else (at input non-space)
        if wasnt in word
          mark it now as word
    if end of line and still marked as inword
      store normalized word in array
      increment count
    return count

### matchingQuery

tries to find matches based on query; returns counter of words
Pseudocode:

    create new counter
    initialize another counter (current)
    loop through words array
      if word is 'or'
        union the counters
        make the current counter as NULL
        continue
      elif 'and'
        continue
      else
        get the counter of the word in the index
        if current is NULL ('or' case)
          make current a new counter
          union word and current counter
        else
          intersect the word and current counter

### rankCounters

Ranks counters via selection sort
Pseudocode:

    count the # of docs in counter
    if more than 0 docs
      loop through docs
        set topScore struct to top count in all of counters
        print the match (score, docId, url)
        set the count of the match to 0 for future iterations (selection sort)

### query

pieces everything together to process query and print results
Pseudocode:

    prompt the user
    while user doesnts CRTL+D
      read the line
      parse the line and get max index of words
      if max index > 0
        print the words
        if query is valid
          get counters that match query
          rank the counters
      keep prompting

## Helper functions

### min

    get smaller number of 2 for counts

### unionHelper

    either set (if not seen) or add (if in counter) the key-count pair to current counter

### unionCtr

    combine 2 counters by iterating through them

### intersectHelper

    set the count of the key to the smaller count of the 2

### intersect

    intersect two counters through iterating

### updateScore

    update score/docID of topScore struct given key/count

Pseudocode:

    if count is greater than topScore's score
      set the key and count to current key and count


### countDocs
count number of docs

Pseudocode:

    if count is greater than 0
      increment the num of docs

### printMatch

    reads URL from file and prints the docs

Pseudocode:

    create filepath
    open file for reading
    read line from file to get URL
    print score, docID, URL



## Other modules

### index

Pseudocode for `index_find`:

    use `hashtable_find` on the index and word


## Function prototypes

### querier

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `querier.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static void parseArgs(int argc, char* argv[]);
static void prompt(void);
static int min(const int a, const int b);
static bool validQuery(char** words, int count);
static int parseQuery(char** words, char* line);
static void unionHelper(void* arg, const int key, const int count);
static void unionCtr(counters_t* allWords, counters_t* currWord);
static void intersectHelper(void* arg, const int key, const int count);
static void intersect(counters_t* allWords, counters_t* currWord);
static counters_t* matchingQuery(char** words, int maxCount, index_t* index);
static void updateScore(void* arg, const int key, const int count);
static void countDocs(void* arg, const int key, const int count);
static void printMatch(char* pageDirectory, const int docID, const int count);
static int rankCounters(counters_t* counter, char* pageDirectory);
static void query(index_t* index, char* pageDirectory);
```

### index

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `../common/index.h` and is not repeated here.

```c
counters_t* index_find(index_t* index char* word);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

We use defensive programming techniques to catch errors internally and exit early.
For instance, we check for correct parameters and if files are opened properly and memory assertation.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There is 1 unit: querier.


### Integration/system testing

We write a script `testing.sh` that invokes the querier several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, we run tests (with some on valgrind) on 3 sites: letters, toscrape, and wikipedia
We also test the querier module using valgrind.