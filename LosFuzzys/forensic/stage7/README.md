# stage7

## Description

Tags: _forensic_

## Recon

I was given a password-protected PDF file called `last_stage.pdf` along with two hints:

```
- If this "reliable" news-site says so, then this has to be the most secure password ever found!
- Even SecLists knows this news-site :)
```

First, I examined the PDF's metadata:

```bash
exiftool Last_Stage.pdf
```

```
ExifTool Version Number         : 13.52
File Name                       : Last_Stage.pdf
Directory                       : .
File Size                       : 16 kB
File Modification Date/Time     : 2026:04:06 15:26:05+02:00
File Access Date/Time           : 2026:04:06 15:26:05+02:00
File Inode Change Date/Time     : 2026:04:06 15:26:05+02:00
File Permissions                : -rw-r--r--
File Type                       : PDF
File Type Extension             : pdf
MIME Type                       : application/pdf
PDF Version                     : 1.7
Linearized                      : No
Encryption                      : Standard V5.6 (256-bit)
Warning                         : [minor] Decryption is very slow for encryption V5.6 or higher
User Access                     : Print, Modify, Copy, Annotate, Fill forms, Extract, Print high-res
```


## Exploitation

The first hint mentions a "reliable" news-site that claims to have found the most secure password ever.
The quotes around "reliable" strongly suggested satire.

In Austria there are well-known satirical news sites like:
- Der Postillon
- Die Tagespresse

I searched Google for:

```
postillon most secure password
```

This led me to a "Der Postillon" article from 2014:
https://www.der-postillon.com/2014/04/sicherstes-passwort.html

The article claims that researchers discovered the "most secure password ever found": `Mb2.r5oHf-0t`

I used this password to unlock the PDF, which then revealed the flag

## Flag

`KDCTF{B3st_PW_EUW}`
