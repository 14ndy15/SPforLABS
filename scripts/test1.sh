#!/bin/bash

for n in {20..60..10}; do
  ./sp.exe -n $n -t 25 -a 0.7 -initialCond >> output.txt
done
