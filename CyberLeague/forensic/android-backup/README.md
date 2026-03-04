# android-backup

## Description
> You got a Backup of the phone of a very bad guy. Can you find the information we need?

Tags: _forensic_

## Recon

I was given an Android backup file called `backup.ab`.

First, I needed to extract the backup file. Android backups are essentially tar archives with a custom header. I used this command to strip the header and decompress the archive:

```bash
( printf "\x1f\x8b\x08\x00\x00\x00\x00\x00" ; tail -c +25 backup.ab ) |  tar xfvz -
```

This command:
- Adds a proper gzip header with `printf`
- Skips the first 24 bytes (Android backup header) using `tail -c +25`
- Extracts the resulting tar.gz archive

After extraction, I explored the filesystem structure and found the browser application data at:
```
apps/com.android.browser/db/browser2.db
```

## Exploitation

The `browser2.db` file is an SQLite database containing the browser's data.

First, I checked what tables were available by querying the schema:

```sql
SELECT * FROM sqlite_master;
```

This revealed several interesting tables:
- `bookmarks`
- `history`
- `searches`
- `settings`

And many more. I started by examining the bookmarks:

```sql
SELECT * FROM bookmarks;
```

Nothing interesting there, so I moved on to the browsing history:

```sql
SELECT * FROM history;
```

In the history table, I found a suspicious Google search entry:

```
andr0id_b4ckup5_ar3_nic3 - Google Search
http://www.google.com/search?redir_esc=&client=ms-android-google&hl=en-US&safe=images&oe=utf-8&q=andr0id_b4ckup5_ar3_nic3&source=android-browser-type&qsubts=1573066872707&devloc=0
```

The search query `andr0id_b4ckup5_ar3_nic3` looked like flag format, which when wrapped in the flag format gave me the flag!

## Flag

`KDCTF{andr0id_b4ckup5_ar3_nic3}`
