# CS50 TSE Indexer Implementation details for Lab 5
## John Wang (thejohnyw), May 2023

### Influenced by crawler's given IMPLEMENTATION.md

## Data structures 

We make a new index module. We use two data structures: a 'hashtable' with keys of word and 'counters' as items (key of docID and count of words in file of matching docID).
Index start empty.
The size of the index (slots) is impossible to determine in advance, so we use 600.

## Control flow

The indexer is implemented in one file `indexer.c`, with four functions.

### main

The `main` function simply calls `parseArgs` and `indexBuild` then `index_save` and `index_delete` (these 2 are from index.c); `indexBuild` calls `indexPage`, then exits zero.

### parseArgs

Given arguments from the command line, make sure they're valid

* if any trouble is found, print an error to stderr and exit non-zero.

### indexPage

Scans a webpage document to add its words to the index
Pseudocode:

 steps through each word of the webpage,
   skips trivial words (less than length 3),
   normalizes the word (converts to lower case),
   looks up the word in the index,
     adding the word to the index if needed
   increments the count of occurrences of this word in this docID

### indexBuild

builds an in-memory index from webpage files it finds in the pageDirectory

Pseudocode:

  creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage

## Other modules

### pagedir


Pseudocode for `pagedir_validate`:

	construct the pathname for the .crawler file in that directory
	open the file for reading; on error, return false.
	close the file and return true.


Pseudocode for `pagedir_getPage`:

    construct the pathname for the page file give pageDirectory and docID
    open that file for reading
    get the URL
    get the depth
    get the contents of the webpage
    make a new webpage with these 3 variables
    close the file
    return the new webpage

### index

Pseudocode for `index_save`:

    open the file for reading; on error, return NULL
    hashtable iterate the index and file to save into file
    close the file

Pseudocode for `index_load`:

    open the file for reading; on error, return false.
    create a new index
    loop through every word in the file
      make new counters
      while it reads docID and count
        set the count of words
      insert word to counters pair into index hashtable
    close the file
    return the index

Pseudocode for `index_delete`:

    delete the index hashtable and every item (by deleting every counter via a deleteCounter method)

Pseudocode for `index_new`:

	return a new hashtable with inputted slot count


### word

Pseudocode for `normalize_word`:

    for each letter in the word
      lowercase it using ctype
    return the lowercased word

### indextest

#### `parseArgs`

Given arguments from the command line, make sure they're valid

#### `main`

use `parseArgs`, load the index, save it to new file


### libcs50

We leverage the modules of libcs50, most notably `counters`, `hashtable`, and `webpage`.
See that directory for module interfaces.


## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `indexer.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static void parseArgs(int argc, char* argv[]);
static void indexPage(webpage_t* page, index_t* index, int docID)
static index_t* indexBuild(char* pageDirectory);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `../common/pagedir.h` and is not repeated here.

```c
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_getPage(const char* pageDirectory, const int docID);
```


### index

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `../common/index.h` and is not repeated here.

```c
index_t* index_new(int slots);
void index_save(index_t* index, char* path);
void index_delete(index_t* index);
index_t* index_load(char* path);
```

### word

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `../common/word.h` and is not repeated here.

```c
char* normalize_word(char* word);
```


## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

We use defensive programming techniques to catch errors internally and exit early.
For instance, we check for correct parameters and if files are opened properly.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are 5 units (indexer, indextest, index, word, and pagedir).
The indexer represents the whole system and is covered below. indextest is below as well.
The pagedir unit is tiny; it could be tested using a small C 'driver' to invoke its functions with various arguments, but it is likely sufficient to observe its behavior during the system test.
The index and word units are also small; similar testing to pagedir



### Integration/system testing

We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, we run tests on moderate-sized test cases (such as `toscrape` at depth 1,2).
Lastly, we run tests with valgrind over a moderate-sized test case (such as `letters` at depth 10).
We also test the indexertest module using valgrind.