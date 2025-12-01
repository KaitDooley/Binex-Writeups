# Day 1 Part 2 Writeup

## Steps:
1. Save the file as input1.txt and process using grep and sort
 ```
$ cat input1.txt | grep -Eo "^[0-9]{5}[^/n]" | sort -n > col1.txt
$ cat input1.txt | grep -Eo " [0-9]{5}" | grep -Eo "[0-9]{5}" | sort -n > col2.txt
```

2. Process in python using solve2.py (see code)


## Solution:
```
Your puzzle answer was 23384288.

Both parts of this puzzle are complete! They provide two gold stars: **
```
