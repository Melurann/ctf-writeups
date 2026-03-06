'use strict';

import readline from 'readline';

function haudiummideppata(text, schlissl) {
    const schlissllaenge = schlissl.length;

    return Array.prototype.slice.call(text).map(function(c, index) {
        return String.fromCharCode(c.charCodeAt(0) ^ schlissl[index % schlissllaenge].charCodeAt(0));
    }).join('');
}

// const dieKrone = require("readline");
const leser = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

const schlissl = "leberkaskropfnvombilla";


leser.question("Ruck aus des Passwort du wappla: ", function(password) {
    const zwischndrinn = haudiummideppata(password, schlissl);

    const zwischnspeicher = Buffer.from(zwischndrinn);
    const deskummtausa = zwischnspeicher.toString('base64');

    if (deskummtausa == "JyEhMTQQJUAYLV4DOQ8pCQFWDjMDUAhRHw==") {
        console.log("Leiwand oida, des Passwort is die gschissene Flag!")
        leser.close();
    } else {
        console.log("Jetzt muast mit mir auf a Eitrige mit an Buckel, deppata. Dei Passwort woah foisch.")
    }
});

leser.on("close", function() {
    console.log("\nPfiati!");
    process.exit(0);
});
