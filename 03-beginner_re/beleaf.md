# Beleaf Writeup

## Steps:
1. Open in ghidra and analyze
	- Identify key functions and variables
	- Notice characters of input are altered then checked against a 'goal' variable
2. Write values of 'goal' variable and decode using 'lookup' global variable
3. Write flag into './beleaf/' to confirm


## Solution:
'''
$ ./beleaf
Enter the flag
>>> flag{we_beleaf_in_your_re_future}
Correct!
'''

