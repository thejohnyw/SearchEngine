/**
 * crawler.c
 * John Wang
 * April 25, 2023
 * File implements the crawler design for TSE
 * Input: there are no file inputs; command line parameters: [seedURL] [pageDirectory] [maxDepth]
 * Output: Per the requirements spec, Crawler will save each explored webpage to a file, one webpage per file, 
 * using a unique documentID as the file name. 
 * Within each of these files, crawler writes: the full page URL on the first line, the depth of the page 
 * (where the seedURL is considered to be depth 0) on the second line, the page contents (i.e., the HTML code),
 * beginning on the third line. See specs for ever more detail
 * Usage: cmd line ->  ./crawler [seedURL] [pageDirectory] [maxDepth]
 * Includes functions:
 * main: runs the program
 * parseArgs: initalize args
 * crawler: crawl page from seedURL to given maxDepth
 * pageScan: scan page for links
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/pagedir.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"

// function to parse/validate cmd line args
/**
 * exit of 1 if any errors: not internal URL, failed to start pageDirectory, invalid max depth
*/
static void
parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth)
{
    // wrong # of args
    if (argc != 4) {
        fprintf(stderr, "Usage error; Wrong # of args; proper use: %s [seedURL] [pageDirectory] [maxDepth]\n", argv[0]);
        exit(1);
    }
    *seedURL = argv[1];
    char* validNormalizedURL = normalizeURL(*seedURL); // normalize from webpage.h
    if (validNormalizedURL == NULL) {
        fprintf(stderr, "Error: invalid seed URL\n");
        exit(1);
    }
    // validate it is internal URL
     if (!isInternalURL(validNormalizedURL)) {
        mem_free(validNormalizedURL);
        fprintf(stderr, "Error: seed URL is not internal\n");
        exit(1);
     }
     mem_free(validNormalizedURL);
     // initialize page directory
     *pageDirectory = argv[2];
     if (!pagedir_init(*pageDirectory)) {
        fprintf(stderr, "Error: could not initialize page directory\n");
        exit(1);
     }
     // validate max depth
     const char* maxDepString = argv[3];
     *maxDepth = 0;
     char* end; // pointer to where cmd line arg is no longer a number
     *maxDepth = (int) strtol(maxDepString, &end, 10); // extract int from cmd line
     // first 2 check if can convert to int; last 2 check for range
     if (end == maxDepString || *end != '\0' || *maxDepth < 0 || *maxDepth > 10) {
        fprintf(stderr, "Error: invalid max depth\n");
        exit(1);
     }
}

// function extracts any internal URLS given webpage and a bag & hashtable
static void
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
   char* url;
   int count = 0;
   const int depth = webpage_getDepth(page);
   printf("%d Scanning: %s\n", depth, webpage_getURL(page));
   // iterate until go thru all URLs
   while ((url = webpage_getNextURL(page, &count)) != NULL) {
      printf("%d    Found: %s\n", depth, url);
      char* normalized = normalizeURL(url);
      // if URL is internal
      if (normalized != NULL && isInternalURL(normalized)) {
         // if inserting succeeded
         if (hashtable_insert(pagesSeen, normalized, "")) {
            // create webpage and insert to bag
            webpage_t* newPage = webpage_new(normalized, webpage_getDepth(page) + 1, NULL);
            bag_insert(pagesToCrawl, newPage);
            printf("%d    Added: %s\n", depth, normalized);
         }
         else { // free if insert error
            printf("%d  IgnDupl: %s\n", depth, normalized);
            mem_free(normalized);
         }
      }
      else { // free if not internal
         printf("%d  IgnExtrn: %s\n", depth, normalized);
         mem_free(normalized);
      }
      // always free current url at end of each iteration
      mem_free(url);
   }
}

// function crawls from 'seedURL' to 'maxDepth 'and saves pages in 'pagedirectory' given these 3
static void 
crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
   const int MAX = 200;
   // use mem_assert to determine if created modules are NULL
   hashtable_t* seen = mem_assert(hashtable_new(MAX), "Print if new hashtable failed");
   bag_t* pagesCrawl = mem_assert(bag_new(), "Print if new bag failed");
   char* urlCopy = mem_malloc(strlen(seedURL)+1);
   strcpy(urlCopy, seedURL);
   // insert a webpage with depth at 0 and seedURL cpy
   bag_insert(pagesCrawl, webpage_new(urlCopy, 0, NULL));
   
   if (!hashtable_insert(seen, seedURL, "")) {
      fprintf(stderr, "Error: inserting hashtable\n");
      exit(1);
   }

   webpage_t* currPage;
   int id = 0;
   // while bag is not empty & extracts a webpage
   while ((currPage = (webpage_t *)bag_extract(pagesCrawl)) != NULL) {
      // if fetch successful
      if (webpage_fetch(currPage)) {
         printf("%d  Fetched: %s\n", webpage_getDepth(currPage), webpage_getURL(currPage));
         id++;
         // save the webpage to directory
         pagedir_save(currPage, pageDirectory, id);
         // scan if not at maxDepth
         if (webpage_getDepth(currPage) < maxDepth) {
            pageScan(currPage, pagesCrawl, seen);
         }
      }
      // delete the webpage
      webpage_delete(currPage);
   }
   hashtable_delete(seen, NULL);
   bag_delete(pagesCrawl, webpage_delete);

}

// main method runs crawl after parsing; return 0 if no errors
int
main(const int argc, char* argv[])
{
   char* seedURL = NULL;
   char* pageDirectory = NULL;
   int maxDepth = 0;
   parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
   crawl(seedURL, pageDirectory, maxDepth);
   return 0;
}