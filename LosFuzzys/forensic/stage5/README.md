# stage5

## Description

Tags: _forensic_

## Recon

I was given a password-protected ZIP file called `stage5.zip` along with two hints:

- What do my password and lord of the ring have in common? One thing to rule them all :)
- ScoobyDoobeyDoo

First, I verified the file type:

```bash
exiftool stage5.zip
```

```
ExifTool Version Number         : 13.52
File Name                       : stage5.zip
Directory                       : .
File Size                       : 227 bytes
File Modification Date/Time     : 2026:03:23 10:22:43+01:00
File Access Date/Time           : 2026:03:23 10:23:57+01:00
File Inode Change Date/Time     : 2026:03:23 10:23:15+01:00
File Permissions                : -rw-r--r--
File Type                       : ZIP
File Type Extension             : zip
MIME Type                       : application/zip
Zip Required Version            : 788
Zip Bit Flag                    : 0x0001
Zip Compression                 : None
Zip Modify Date                 : 2024:06:05 16:17:44
Zip CRC                         : 0xc3d09da9
Zip Compressed Size             : 63
Zip Uncompressed Size           : 63
Zip File Name                   : stage5_flag.txt
```

The first hint about "One thing to rule them all" was cryptic. After googling _"hashcat one thing to rule them all"_, I discovered it refers to [OneRuleToRuleThemAll](https://github.com/stealthsploit/OneRuleToRuleThemStill/) - a famous password cracking ruleset for hashcat!

## Exploitation

First, I extracted the hash from the ZIP file:

```bash
zip2john stage5.zip > stage5.hash
```

The output from `zip2john` isn't directly compatible with hashcat. I needed to clean it up by removing everything before the first `$` and everything after the last `$`, saving the cleaned hash to `hash.txt`.

I downloaded the OneRuleToRuleThemStill ruleset (a newer version of OneRuleToRuleThemAll) from GitHub:

```bash
git clone https://github.com/stealthsploit/OneRuleToRuleThemStill/
```

Then I ran hashcat with:
- `-m 17225` (PKZIP mode)
- `-a 0` (dictionary attack)

```bash
hashcat -m 17225 -a 0 hash.txt $(wordlists_path) -r OneRuleToRuleThemStill/OneRuleToRuleThemStill.rule
```

After a few minutes, hashcat successfully cracked the password: `Sco5obyD`

I extracted the ZIP file using this password and retrieved the flag from `stage5_flag.txt`.

Amusingly, the flag itself was a YouTube link to a hardstyle Scooby-Doo theme song :)

## Flag

`KDCTF{https://www.youtube.com/watch?v=FuVI6R4hAIc}`
