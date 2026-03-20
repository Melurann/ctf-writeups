# stage1

## Description

Tags: _forensic_

## Recon

I was given a file called `stage1.zip`, which was password-protected.

First, I examined the ZIP file's metadata using `exiftool`:

```bash
exiftool stage1.zip
```

```
ExifTool Version Number         : 13.52
File Name                       : stage1.zip
Directory                       : .
File Size                       : 3.6 kB
File Modification Date/Time     : 2026:03:18 11:55:45+01:00
File Access Date/Time           : 2026:03:18 11:58:34+01:00
File Inode Change Date/Time     : 2026:03:18 11:58:01+01:00
File Permissions                : -rw-r--r--
File Type                       : ZIP
File Type Extension             : zip
MIME Type                       : application/zip
Zip Required Version            : 788
Zip Bit Flag                    : 0x0001
Zip Compression                 : None
Zip Modify Date                 : 2024:06:05 17:14:22
Zip CRC                         : 0x17c0c99a
Zip Compressed Size             : 38
Zip Uncompressed Size           : 38
Zip File Name                   : stage1_flag.txt
Warning                         : [minor] Use the Duplicates option to extract tags for all 2 files
```

The metadata revealed that the ZIP contains a file called `stage1_flag.txt`, and the challenge hint stated: **"Second Worst Password"**.

This hint suggested that the password would be a commonly used weak password.

## Exploitation

I used John the Ripper to crack the ZIP password. First, I set up the environment with the necessary tools:

```bash
nix-shell -p john wordlists
```

John the Ripper requires the ZIP file to be converted to a hash format first:

```bash
zip2john stage1.zip > stage1.hash
```

Then I ran John with the rockyou.txt wordlist:

```bash
john --wordlist=$(wordlists_path)/rockyou.txt stage1.hash
```

```
Created directory: /home/fruroa/.john
Created directory: /home/fruroa/.john/opencl
Using default input encoding: UTF-8
Loaded 1 password hash (PKZIP [32/64])
Will run 12 OpenMP threads
Press 'q' or Ctrl-C to abort, 'h' for help, almost any other key for status
password         (stage1.zip)
1g 0:00:00:00 DONE (2026-03-20 08:39) 100.0g/s 2457Kp/s 2457Kc/s 2457KC/s 123456..280690
Use the "--show" option to display all of the cracked passwords reliably
Session completed.
```

The password was cracked almost instantly: `password`

I then extracted the ZIP file using this password and retrieved the flag from [stage1_flag.txt](./stage1_flag.txt).

## Flag

`KDCTF{W3ll_th4t_w4s_3asy}`
