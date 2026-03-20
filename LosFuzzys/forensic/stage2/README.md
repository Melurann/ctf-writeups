# stage2

## Description

Tags: _forensic_

## Recon

I was given a password-protected file called `stage2.rar`.

Following the same approach as stage1, I first tried to extract the hash for password cracking:

```bash
rar2john stage2.rar > stage2.hash
```

```
! stage2.rar: Not a RAR file
```

Despite having a `.rar` extension, it wasn't actually a RAR file. I examined the file's metadata to determine its format:

```bash
exiftool stage2.rar
```

```
ExifTool Version Number         : 13.52
File Name                       : stage2.rar
Directory                       : .
File Size                       : 3.3 kB
File Modification Date/Time     : 2024:06:05 17:29:08+02:00
File Access Date/Time           : 2026:03:20 08:52:33+01:00
File Inode Change Date/Time     : 2026:03:20 08:52:13+01:00
File Permissions                : -rw-r--r--
File Type                       : 7Z
File Type Extension             : 7z
MIME Type                       : application/x-7z-compressed
File Version                    : 7z v0.04
Modify Date                     : 2024:06:05 17:24:57+02:00
Archived File Name              : ky5_s
Warning                         : [minor] Use the Duplicates option to extract tags for all 3 files
```

The metadata revealed that the file is actually a 7-Zip archive.

## Exploitation

I renamed the file to the correct extension and attempted to crack it with John the Ripper:

```bash
mv stage2.rar stage2.7z
7z2john.pl stage2.7z > stage2.hash
john --wordlist=$(wordlists_path)/rockyou.txt stage2.hash
```

```
Using default input encoding: UTF-8
Loaded 1 password hash (7z, 7-Zip archive encryption [SHA256 128/128 SSE2 4x AES])
Cost 1 (iteration count) is 524288 for all loaded hashes
Cost 2 (padding size) is 0 for all loaded hashes
Cost 3 (compression type) is 2 for all loaded hashes
Cost 4 (data length) is 80 for all loaded hashes
Will run 12 OpenMP threads
Press 'q' or Ctrl-C to abort, 'h' for help, almost any other key for status
...
0g 0:00:37:11 1.70% (ETA: 2026-03-21 21:44) 0g/s 128.0p/s 128.0c/s 128.0C/s 150223..14376
Session aborted
```

After over 37 minutes with only 1.70% progress, it was clear the password wasn't in the rockyou.txt wordlist.


I reconsidered the exiftool output and noticed the `Archived File Name: ky5_s` field.

I tried extracting the archive using this value:

```bash
7z x stage2.7z
```

When prompted for the password, I entered `ky5_s` and the archive extracted successfully and revealed the flag.

## Flag

`KDCTF{B4ng3d_my_h3ad_0n_th3_k3yb0ard}`
