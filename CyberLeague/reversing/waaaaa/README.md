# waaaaa

## Description
> waaaaa

Tags: _reversing_

## Recon

I was given an HTML file that loads and executes a WebAssembly (WASM) module to validate user input.

The HTML file contains a simple interface:
- An input field to enter text
- A "Validate" button that calls the WASM validation function
- A result display showing either "Valid" or "Invalid"

The JavaScript code shows how the validation works:
```js
const encodedInput = encoder.encode(input);
const ptr = wasmInstance.allocate(encodedInput.length);
const buffer = new Uint8Array(memory.buffer, ptr, encodedInput.length);
buffer.set(encodedInput);
const result = wasmInstance.validate(ptr, encodedInput.length);
```

The user input is converted to bytes, written to WASM memory, and then validated. To find the correct flag, I needed to reverse engineer the WASM binary to understand what string it expects.

## Exploitation

First, I converted the binary WASM file to WebAssembly Text format (WAT) for easier analysis:

```bash
wasm2wat validate.wasm > validate.wat
```

Opening the WAT file, I found the `$val` function that performs the validation. The function follows this pattern:

The first check verifies that the input is exactly 20 characters long:

```wasm
i32.const 20           # Expected length
local.set 6
local.get 5            # Actual length
local.get 6
i32.ne                 # Check if not equal to 20
```


The code then checks each position individually. For most positions, it loads a byte from the string and compares it to an expected ASCII value:

Positions 0-3 (Flag Prefix):
```wasm
i32.load8_u            # Load position 0
i32.const 67           # Expected: ASCII 67 = 'C'
i32.ne                 # Check if not equal
```

Following this pattern through the code:
- Position 0: `67` → `'C'`
- Position 1: `76` → `'L'`
- Position 2: `65` → `'A'`
- Position 3: `123` → `'{'`

Positions 4-13 (Main Content):
```wasm
offset=4  → i32.const 87  → 'W'
offset=5  → i32.const 65  → 'A'
offset=6  → i32.const 83  → 'S'
offset=7  → i32.const 77  → 'M'
offset=8  → i32.const 69  → 'E'
offset=9  → i32.const 88  → 'X'
offset=10 → i32.const 80  → 'P'
offset=11 → i32.const 79  → 'O'
offset=12 → i32.const 82  → 'R'
offset=13 → i32.const 84  → 'T'
```
This spells: `WASMEXPORT`

Position 14:
```wasm
offset=14 → i32.const 95 → '_'
```

Position 15 (XOR):
```wasm
i32.load8_u offset=15
i32.const 17           # XOR with 17
i32.xor
i32.const 32           # Result must equal 32
i32.ne
```
To solve: `char XOR 17 = 32`, therefore `char = 32 XOR 17 = 49 = '1'`

Position 16 (Subtraction):
```wasm
i32.load8_u offset=16
i32.const 11           # Subtract 11
i32.sub
i32.const 40           # Result must equal 40
i32.ne
```
To solve: `char - 11 = 40`, therefore `char = 51 = '3'`

Positions 17-19:
```wasm
offset=17 → i32.const 65  → 'A'
offset=18 → i32.const 57  → '9'
```

Position 20:
The code checks the last position (length - 1) for the closing brace:
```wasm
i32.const 125          # ASCII 125 = '}'
```

Putting all the pieces together:
```
CLA{WASMEXPORT_13A9}
```

I entered this into the HTML page and received "Valid"

## Flag

`CLA{WASMEXPORT_13A9}`
