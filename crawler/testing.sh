# !/bin/bash
# John Wang, testing.sh for ./crawler
# usage: run testing.sh
# no inputs; output writes results

mkdir ../data

echo TESTING with no args: should print error
./crawler

echo TESTING with non-seed/non-internal URL: should print error
mkdir ../data/bad
./crawler http://youtube.com ../data/bad 1

echo TESTING with wrong pageDirectory: should print error
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html wrongDirect 1

echo TESTING letters at depth 11: should print error for exceeding max depth
mkdir ../data/letters-11
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-11 11

echo TESTING letters at depth -1: should print error for bad max depth
mkdir ../data/letters--1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters--1 -1


echo TESTING letters at depth 0: should exit 0
mkdir ../data/letters-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 0

echo TESTING letters at depth 1: should exit 0
mkdir ../data/letters-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-1 1

echo TESTING letters at depth 2: should exit 0
mkdir ../data/letters-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 2

echo TESTING letters at depth 10: should exit 0
mkdir ../data/letters-10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-10 10

echo TESTING toscrape at depth 0: should exit 0
mkdir ../data/toscrape-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-0 0

echo TESTING toscrape at depth 1 with valgrind: should exit 0
# TESTING VALGRIND
bash valgrind.sh

echo TESTING wikipedia at depth 0: should exit 0
mkdir ../data/wikipedia-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-0 0

echo TESTING wikipedia at depth 1: should exit 0
mkdir ../data/wikipedia-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-1 1



