# shella-easy Writeup

## Steps
Same as pwn3 except there is a variable that needs to be preserved (look at ghidra). Stack address is still provided and stack variable layout can be identified the same way.

## Solution 
```
$ python3 ex_shella-easy.py
[+] Starting local process './shella-easy': pid 37673
[*] Switching to interactive mode

$ ls
ex_shella-easy.py  shella-easy
$ w
 13:19:10 up 20:36,  2 users,  load average: 1.04, 2.49, 16.58
USER     TTY      FROM             LOGIN@   IDLE   JCPU   PCPU WHAT
kali              -                Wed06    2:49   0.00s  0.09s lightdm --session-child 13 24
kali              -                Wed06    2:49   0.00s  0.81s /usr/lib/systemd/systemd --user
$ exit
[*] Got EOF while reading in interactive
$ 
[*] Process './shella-easy' stopped with exit code 0 (pid 37673)
[*] Got EOF while sending in interactive
```                                           
