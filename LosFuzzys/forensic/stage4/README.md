# stage4

## Description

Tags: _forensic_

## Recon

I was given a KeePass password database file called `Database.kdbx` along with a hint file containing:

```
A?l?d?s?l
```

KeePass databases require a master password to unlock. My first instinct was to try using the hint directly as the password.

I installed KeePass to attempt opening the database:

```bash
nix-shell -p keepass
```

I opened `Database.kdbx` in KeePass and entered `A?l?d?s?l` as the master password, but it failed.

Looking at the hint more carefully, I recognized the pattern: this is hashcat mask syntax

Mask syntax breakdown:
- `A` = literal character 'A'
- `?l` = lowercase letter
- `?d` = digit
- `?s` = special character
- `?l` = lowercase letter

So the password follows the pattern: `A[lowercase][digit][special][lowercase]`

## Exploitation

First, I converted the KeePass database to a hash format that John the Ripper can process:

```bash
keepass2john Database.kdbx > keepass.hash
```

Then I used John the Ripper with the mask attack mode, using the hint as the mask pattern:

```bash
john --format=keepass --mask='A?l?d?s?l' keepass.hash
```

```
Using default input encoding: UTF-8
Loaded 1 password hash (KeePass [SHA256 AES 32/64])
Cost 1 (iteration count) is 600000 for all loaded hashes
Cost 2 (version) is 2 for all loaded hashes
Cost 3 (algorithm [0=AES 1=TwoFish 2=ChaCha]) is 0 for all loaded hashes
Will run 12 OpenMP threads
Press 'q' or Ctrl-C to abort, 'h' for help, almost any other key for status
Ap5!v            (Database)
1g 0:01:18:00 DONE (2026-03-20 18:46) 0.000214g/s 36.82p/s 36.82c/s 36.82C/s Aw8!v..Ag4!v
Use the "--show" option to display all of the cracked passwords reliably
Session completed.
```

After about 78 minutes, the password was cracked: `Ap5!v`

I then opened the KeePass database with this master password and retrieved the flag from inside.

## Flag

`KDCTF{KeePass2John_h3h3_kekw}`
