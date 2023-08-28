#!/bin/bash
# John Wang, valgrind.sh : testing valgrind for crawler
# usage: run valgrind.sh
# no inputs; output writes results

echo TESTING toscrape with depth of 1 with valgrind: should print valgrind results and 'correctly exit 0'
mkdir ../data/toscrape-val-1
if valgrind ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-val-1 1 ; then
    echo correctly exit 0
fi