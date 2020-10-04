#!/bin/sh

# Authors: Dylan McClure, Eliot Shea, Aidan Martin
# Last Change: 03 October 2020

# make clean to remove any cruft
make clean &> /dev/null

# make; if errors, exit prematurely with error
make > /dev/null || {
  tput bold
  tput setaf 1
  echo Make failed
  exit 1
}

# strace > {empty,part1}.trace; if errors, exit
for name in {empty,part1}
do
strace -o "$name".trace ./"$name".x &> /dev/null || {
  tput bold
  tput setaf 1
  echo strace ./"$name".x failed
  exit 2
}
done

# get number of additional lines in part1.trace
d="$(($(wc -l < part1.trace) - $(wc -l < empty.trace)))"

# print results
tput bold
echo Make and straces succeeded\; Results:
if [[ $d -eq 7 ]]
then
  # exactly 7 more lines; print perfect
  tput setaf 2
  echo "Perfect; there are 7 additional lines"
else
  # not 7 additional lines; print error and difference
  tput setaf 1
  echo "Error; must be 7 more lines in part1.trace than in empty.trace; instead found $d"
fi
tput sgr 0

