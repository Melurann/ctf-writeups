from pwn import *
import ctypes
import base64

username = "rfruth"
seed = 0x1337c0de
key = p64(-0x21523f2141102153 & 0xffffffffffffffff)

libc = ctypes.CDLL("libc.so.6")
libc.srand(seed)

userbytes = list(username.encode())
userln = len(userbytes)

for i in range(userln):
    j = libc.rand() % userln
    userbytes[i], userbytes[j] = userbytes[j], userbytes[i]

xored = bytearray()
for i, byte in enumerate(userbytes):
    xored.append(byte ^ key[i % 8])

print(base64.b64encode(xored).decode())
