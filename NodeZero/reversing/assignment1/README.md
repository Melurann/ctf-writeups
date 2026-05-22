# assignment1

## Description

Tags: _reversing_

## Recon

I was given a binary file called `main` that appeared to be a key validation program.

Running the binary, I saw it asks for a name and then a key:
```
Enter your name: test
Hello test
Enter your key:
```

I opened the binary in Binary Ninja and switched to the pseudo-C view to analyze the decompiled code.

### Seeding

The program is using a random number generator with a hardcoded seed `0x1337c0de`:

```c
int64_t rax = *(uint64_t*)((char*)fsbase + 0x28);
srand(0x1337c0de);

printf("Hello %s\n", &var_258);
int64_t var_218;
__builtin_memset(&var_218, 0, 0x100);
printf("Enter your key: ");
fgets(&var_218, 0xff, stdin);
```

### Username Shuffling

The program gets the length of the username and shuffles it using the seeded random number generator:

```c
int32_t rax_15 = strlen(&var_258);

for (int32_t i = 0; i < rax_15; i += 1) {
    int32_t temp1_1 = (int64_t)rand() % rax_15;
    char rax_20 = *(uint8_t*)(&var_258 + (int64_t)i);
    *(uint8_t*)(&var_258 + (int64_t)i) = *(uint8_t*)(&var_258 + (int64_t)temp1_1);
    *(uint8_t*)(&var_258 + (int64_t)temp1_1) = rax_20;
}
```

### XOR Encryption

The shuffled username is then XORed with a hardcoded key:

```c
int64_t var_268 = -0x21523f2141102153;

for (int32_t i_1 = 0; i_1 < rax_15; i_1 += 1)
    *(uint8_t*)(&var_258 + (int64_t)i_1) ^= 
        *(uint8_t*)(&var_268 + ((uint64_t)i_1 & 7));
```

The XOR key is `-0x21523f2141102153`, which cycles through 8 bytes.

### Base64 Encoding

Finally, the XORed result is Base64 encoded:

```c
b64_encode(&var_258, rax_15, &var_118);
```

This encoded string is what the program expects as the "key" input.

## Exploitation

I created a [Python script](./app.py) that mimics the exact algorithm.
Running this script with the username `rfruth` generated the key: `y6ybzLa1`

I then ran the binary with these credentials:

```bash
./main
Enter your name: rfruth
Hello rfruth
Enter your key: y6ybzLa1
You win!
```
