# Day 1 Part 1 Writeup

## Steps:
1. Save the file as input1.txt and process using grep and sort
 ```
$ cat input1.txt | grep -Eo "^[0-9]{5}[^/n]" | sort -n > col1.txt
$ cat input1.txt | grep -Eo " [0-9]{5}" | grep -Eo "[0-9]{5}" | sort -n > col2.txt
```

2. Copy columns into google sheets and get the sum of the absolute values of the differences.


## Solution:
```
Your actual left and right lists contain many location IDs. What is the total distance between your lists?
Your puzzle answer was 2176849.

That's the right answer! You are one gold star closer to finding the Chief Historian. [Continue to Part Two]
```
