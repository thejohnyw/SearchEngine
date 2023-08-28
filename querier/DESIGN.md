# CS50 TSE Querier
## John Wang
## Design Spec
### Influenced from given indexer design

According to the Querier Requirements Spec, the TSE *querier* is a standalone program that responds to queries by users. Querier loads an index and searches for pages that include the inputted keywords.

### User interface

The querier's only interface with the user is on the command-line; it must always have two arguments.

``` bash
./querier pageDirectory indexFilename
```

For example, if `letters` is a pageDirectory in `../data`,

``` bash
$ ./querier ../data/letters ../data/letters-index
```

### Inputs and outputs

**Input**: the querier reads files from a directory by constructing file pathnames from the `pageDirectory` parameter followed by a numeric document ID (as described in the Requirements). It also reads the given index file for that crawler directory.


**Output**: We print to the user a ranked list of pages that match the user-inputted keywords see REQUIREMENTS.md for details on ranking.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and calls other modules;
 2. *prompt*, which asks user for query
 3. *validQuery*, which ensures that the user uses 'and' and 'or' correctly in their given queries.
 4. *parseQuery*, which builds an array of words (query) and calculates the # of words
 5. *matchingQuery*, which processes the array of words based on operators and returns a counter with scores
 6. *unionCtr* & *unionHelper*, helper functions to union 2 counters for 'or' operations
 7. *intersect* & *intersectHelper* & *min*, helper functions to intersect 2 counters for 'and' operations
 8. *updateScore* & *countDocs*, helper functions for iterators; updateScore updates the given counter to have the doc with highest score. countDocs counts valid docs (count>0)
 9. *printMatch*, helper to open a file, get the url, and print the score, doc and url
 10. *rankCounters*, which sorts the given counter in descending order
 11. *query*, which combines many modules to prompt the user, read from stdin, and print ranked matches



And some helper modules that provide data structures:

 1. *index* of files that query looks through;
 2. *counters*, this holds docID and count for each word and will hold the result of webpages that match for the query 

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validate
    parameters, initialize other modules
    build the index off the cmd line given pathname 
    call query, with the index and pageDirectory
    delete the index

where *query:*

    prompt the user
    while user doesnt enter CTRL+D
      parseQuery() of the entered line
      if size of words isn't 0
        print the query
        if it is a validQuery()
          make the counters from matchingQuery()
          rank the counters using rankCounters()
      keep prompt()-ing



### Major data structures

 1. *index* of files that query looks through;
 2. *counters*, this holds docID and count for each word and will hold the result of webpages that match for the query 

### Testing plan

*Integration testing*.  The *querier*, as a complete program, will be tested by using `fuzzquery.c` to generate relevant queries and pipe them to `querier.c`

  1. Test `querier` with various invalid arguments.
	2. no arguments
	3. one argument
	4. three or more arguments
	5. invalid `pageDirectory` (non-existent path)
	6. invalid `indexFile` (non-existent path) 7. invalid `indexFile` (cannot read from)

0. Run *querier* on a variety of pageDirectories and indexFiles including letters, toscrape, and wikipedia with various number of tests and seeds
0. Run *valgrind* on some test cases