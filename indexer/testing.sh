# !/bin/bash
# John Wang, CS50 Lab 5, testing script for indexer
# Usage: ./testing.sh

mkdir ../data/index_outputs

# Test 1.1: Wrong # of args; Should print error/usage
./indexer ../data/letters-10

# Test 1.2: Wrong # of args (too many); Should print error/usage
./indexer ../data/letters-10 imaginaryDirect ../data/index_outputs/imagine

# Test 1.3: Wrong # of args (indextest); Should print error/usage
./indextest ../data/index_outputs/letters-2-indexer


# Test 2: nonexisting directory; Should print error (should print directory not made by crawler since it doesn't exist)
./indexer imaginaryDirect ../data/index_outputs/imagine

# Test 3: existing but invalid directory (one without .crawler); Should print error
mkdir ../data/imaginary
./indexer ../data/imaginary ../data/imaginary/imagine

# Test 4: test on wikipedia depth 1, but unwritable index file; should print file error
touch ../data/index_outputs/wikipedia-1-indexer
chmod -w ../data/index_outputs/wikipedia-1-indexer
./indexer ../data/wikipedia-1 ../data/index_outputs/wikipedia-1-indexer

# Test 5: test on toscrape depth 1; shouldn't print error
./indexer ../data/toscrape-val-1 ../data/index_outputs/toscrape-val-1-indexer

# Test 6: test on letters depth 2; shouldn't print error
./indexer ../data/letters-2 ../data/index_outputs/letters-2-indexer

# Test 7: test on wikipedia depth 1; shouldn't print error
chmod +w ../data/index_outputs/wikipedia-1-indexer
./indexer ../data/wikipedia-1 ../data/index_outputs/wikipedia-1-indexer

# Test 8: test on wikipedia depth 0; shouldn't print error
./indexer ../data/wikipedia-0 ../data/index_outputs/wikipedia-0-indexer


# Valgrind test 1 (on indextest): Show no leaks
valgrind --leak-check=full --show-leak-kinds=all -s ./indextest ../data/index_outputs/letters-2-indexer ../data/index_outputs/letters-2-indexer-new

# Valgrind test 2: Show no leaks
valgrind --leak-check=full --show-leak-kinds=all -s ./indexer ../data/letters-10 ../data/letters-indexer-10

# Valgrind test 3: Show no leaks
valgrind --leak-check=full --show-leak-kinds=all -s ./indexer ../data/letters-2 ../data/letters-indexer-2