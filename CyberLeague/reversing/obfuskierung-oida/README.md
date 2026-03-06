# obfuskierung-oida

## Description
> obfuscation is getting creative these days.

Tags: _reversing_

## Recon

I was given a [Windows Script](./challenge.ws) file. At the top of the file, I noticed a `'use strict'` indicator, which suggested this was JavaScript/TypeScript code.

However, the code itself was heavily obfuscated using Austrian dialect terms instead of normal JavaScript keywords. Here are the mappings I discovered:

```js
// Austrian Dialect: JavaScript
'HACKL AMOI WOS': function
'FIX OIDA': const
'OIDA': const
'WENNST MANST': =
'WOS WÜSTN': if
'A SCHO WUASCHT': else
'DES GEHT SI SCHO AUS': ==
'I MAN JA NUR': console.log
'DES IS MA ECHT Z'DEPPAT': process.exit
'DRAH DI HAM': return
```

Using these translations, I deobfuscated the script and saved it as [main.js](./main.js) to better understand its structure.

## Exploitation

After deobfuscation, I identified the key function in the code, an XOR encryption function:

```js
function haudiummideppata(text, schlissl) {
    const schlissllaenge = schlissl.length;
    return Array.prototype.slice.call(text).map(function(c, index) {
        return String.fromCharCode(c.charCodeAt(0) ^ schlissl[index % schlissllaenge].charCodeAt(0));
    }).join('');
}
```

The script also contained:
- An encryption key: `"leberkaskropfnvombilla"`
- Encrypted data (Base64): `"jyehmtqqjuaylv4doq8pcqfwdjmduahrhw=="`

Since XOR encryption is symmetric (encrypting twice with the same key returns the original), I used the same function to decrypt the flag:

```js
function haudiummideppata(text, schlissl) {
    const schlissllaenge = schlissl.length;
    return Array.prototype.slice.call(text).map(function(c, index) {
        return String.fromCharCode(c.charCodeAt(0) ^ schlissl[index % schlissllaenge].charCodeAt(0));
    }).join('');
}

const schlissl = "leberkaskropfnvombilla";
const deskummtausa = "jyehmtqqjuaylv4doq8pcqfwdjmduahrhw==";
const encrypted = Buffer.from(deskummtausa, 'base64').toString();
const password = haudiummideppata(encrypted, schlissl);

console.log("Flag:", password);
```

Running this script successfully decrypted the flag

## Flag

`KDCTF{d3s_1s_a_fl4g_o1d4}`
