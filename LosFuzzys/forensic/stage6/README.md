# stage6

## Description

Tags: _forensic_

## Recon

I was given a password-protected PDF file called `flag.pdf` along with two hints:

- CeWL htl-kaindorf.at (LotR-Fashion)
- NTLM: 7C864F433FF30ADCBD1D166471368DD0

## Exploitation

First, I extracted the PDF hash for cracking:

```bash
pdf2john.pl flag.pdf > flag.hash
```

I cleaned up the hash file by removing everything before the first `$` and after the last `$`, saving it as `hash.txt`.

Then I attempted to crack it with the standard rockyou.txt wordlist:

```bash
hashcat -a 0 hash.txt $(wordlists_path)/rockyou.txt
```

```
Session..........: hashcat
Status...........: Exhausted
Hash.Mode........: 10500 (PDF 1.4 - 1.6 (Acrobat 5 - 8))
...
Recovered........: 0/1 (0.00%) Digests (total), 0/1 (0.00%) Digests (new)
Progress.........: 14344384/14344384 (100.00%)
```

No luck, the password wasn't in rockyou.txt.

I researched the cryptic hints:

CeWL: I discovered this is a Custom Word List generator tool that spiders websites and can create wordlists from the content found.

LotR-Fashion: This references "Lord of the Rings Fashion," which probably points to the OneRuleToRuleThemAll rule file used in stage5.


Using CeWL, I generated a custom wordlist from the htl-kaindorf.at website:

```bash
cewl -d 2 -m 5 -w wordlist.txt https://htl-kaindorf.at
```

Parameters:
- `-d 2` - Spider depth of 2 levels
- `-m 5` - Minimum word length of 5 characters
- `-w wordlist.txt` - Output to wordlist.txt

```bash
hashcat -a 0 hash.txt wordlist.txt -r OneRuleToRuleThemStill/OneRuleToRuleThemStill.rule
```

The rule file applied various transformations (capitalization, character substitution, appending numbers, etc.) to the words in the custom wordlist.

Password: `Volleyballmeisterschften2024`

I then opened the PDF with this password and retrieved the flag.

## Flag

`KDCTF{Rul3d_th3_Voll3y_24}`
