# stage3

## Description

Tags: _forensic_

## Recon

I was given a file called `stage3.zip`, another password-protected archive.

Following the pattern from previous stages, I first verified the file type using `exiftool`:

```bash
exiftool stage3.zip
```

```
ExifTool Version Number         : 13.52
File Name                       : stage3.zip
Directory                       : .
File Size                       : 3.2 kB
File Modification Date/Time     : 2024:06:05 17:17:34+02:00
File Access Date/Time           : 2026:03:20 10:12:59+01:00
File Inode Change Date/Time     : 2026:03:20 10:11:38+01:00
File Permissions                : -rw-r--r--
File Type                       : ZIP
File Type Extension             : zip
MIME Type                       : application/zip
Zip Required Version            : 788
Zip Bit Flag                    : 0x0001
Zip Compression                 : None
Zip Modify Date                 : 2024:06:05 17:14:42
Zip CRC                         : 0x9b24b0b6
Zip Compressed Size             : 2714
Zip Uncompressed Size           : 2714
Zip File Name                   : Database.kdbx
Warning                         : [minor] Use the Duplicates option to extract tags for all 3 files
```

The metadata revealed it is indeed a ZIP archive.

## Exploitation

I extracted the hash from the ZIP file for password cracking:

```bash
zip2john stage3.zip > stage3.hash
```

Then I ran John the Ripper with the rockyou.txt wordlist:

```bash
john --wordlist=$(wordlists_path)/rockyou.txt stage3.hash
```

```
Using default input encoding: UTF-8
Loaded 1 password hash (PKZIP [32/64])
Will run 12 OpenMP threads
Press 'q' or Ctrl-C to abort, 'h' for help, almost any other key for status
sexymama         (stage3.zip)
1g 0:00:00:00 DONE (2026-03-20 12:35) 100.0g/s 2457Kp/s 2457Kc/s 2457KC/s 123456..280690
Use the "--show" option to display all of the cracked passwords reliably
Session completed.
```

The password was cracked instantly: `sexymama`

I extracted the ZIP file using this password, which revealed the `stage3_flag.txt` file.

## Flag

`KDCTF{Mothers_I_Like_to_Flag}`
