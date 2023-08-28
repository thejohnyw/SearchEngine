# testing.sh - testing script for querier
# John Wang, CS50, Lab 6 Querier
# usage: ./testing.sh

# paths of sample crawler and index output directories/files
dir=/thayerfs/courses/23spring/cosc050/students_files/tse/output
pageDirectory=$dir/toscrape-2
index=$dir/toscrape-2.index

# testing to see that errors function

# no args; should get usage message/error
./querier

# only 1 arg; should get usage message/error
./querier $pageDirectory

# too many args; should get usage message/error
./querier $pageDirectory $index last

# invalid index file arg; should get error
./querier $pageDirectory wrong

# non-readable index file arg; should get error
touch wrong2
chmod -r wrong2
./querier $pageDirectory wrong2
rm -r wrong2


# invalid pagedirectory file arg; should get error
./querier wrong $index

# Testing for correctness

# Test on toscrape with valgrind: show no memory leaks
valgrind --leak-check=full --show-leak-kinds=all -s ./fuzzquery $index 10 0 | ./querier $pageDirectory $index

# Test with diff # of tests and seed
./fuzzquery $index 5 2 | ./querier $pageDirectory $index

# Test on letters
./fuzzquery ../data/index_outputs/letters-2-indexer 5 2 | ./querier ../data/letters-2 ../data/index_outputs/letters-2-indexer

# Test on letters with diff # of tests and seed
./fuzzquery ../data/index_outputs/letters-2-indexer 10 0 | ./querier ../data/letters-2 ../data/index_outputs/letters-2-indexer

# Testing valgrind for letters depth 10
valgrind --leak-check=full --show-leak-kinds=all -s ./fuzzquery ../data/letters-indexer-10 10 0 | ./querier ../data/letters-10 ../data/letters-indexer-10

# Testing on wikipedia
./fuzzquery ../data/index_outputs/wikipedia-1-indexer 5 1 | ./querier ../data/wikipedia-1 ../data/index_outputs/wikipedia-1-indexer

# Testing with diff # of tests and seed
./fuzzquery ../data/index_outputs/wikipedia-1-indexer 10 0 | ./querier ../data/wikipedia-1 ../data/index_outputs/wikipedia-1-indexer
