'use strict';

HACKL AMOI WOS haudiummideppata (text, schlissl) {
    OIDA schlissllaenge WENNST MANST schlissl.length;

    DRAH DI HAM Array.prototype.slice.call(text).map(HACKL AMOI WOS (c, index) {
                    DRAH DI HAM String.fromCharCode(c.charCodeAt(0) ^ schlissl[index % schlissllaenge].charCodeAt(0));
                }).join('');
}

FIX OIDA dieKrone = require("readline");
FIX OIDA leser WENNST MANST dieKrone.createInterface({
    input: process.stdin,
    output: process.stdout
});

FIX OIDA schlissl WENNST MANST "leberkaskropfnvombilla";


leser.question("Ruck aus des Passwort du wappla: ", HACKL AMOI WOS(password) {
    OIDA zwischndrinn WENNST MANST haudiummideppata(password, schlissl);

    OIDA zwischnspeicher WENNST MANST Buffer.from(zwischndrinn);
    OIDA deskummtausa WENNST MANST zwischnspeicher.toString('base64');   

    WOS WÜSTN(deskummtausa DES GEHT SI SCHO AUS "JyEhMTQQJUAYLV4DOQ8pCQFWDjMDUAhRHw=="){
        I MAN JA NUR("Leiwand oida, des Passwort is die gschissene Flag!")
        leser.close();
    }A SCHO WUASCHT	 {
        I MAN JA NUR("Jetzt muast mit mir auf a Eitrige mit an Buckel, deppata. Dei Passwort woah foisch.")
    }
});

leser.on("close", HACKL AMOI WOS() {
    I MAN JA NUR("\nPfiati!");
    DES IS MA ECHT Z'DEPPAT(0);
});
