# white-hackeru-des

## Description
> The japanese love cringy songs. But something is different here, can you find out what? Maybe there is a password or anything similar.

Tags: _forensic_

## Recon

I was given a video file called [song.mp4](./song.mp4) containing a Japanese song.

My first instinct was to check if there was any hidden information in the audio. I used OpenAI's Whisper to transcribe and translate the Japanese lyrics:

```bash
nix-shell -p openai-whisper ffmpeg
whisper japanese.wav --model medium --language Japanese --task translate
```

The [translation](./translation.txt) was amusing but didn't contain any useful information for the challenge.

Next, I examined the file's metadata using `exiftool`:

```bash
exiftool song.mp4
```

```
Title                           : Whitehacker!
Content Create Date             : 0
Encoder                         : Lavf57.83.100
Comment                         : SGKhwEQIyMzeqKgZwiqA
Warning                         : Unknown trailer with truncated '3\x00\x01\x00' data at offset 0x2e9dc46
Image Size                      : 1280x720
...
```

The `Comment` field contained a suspicious Base64-looking string: `SGKhwEQIyMzeqKgZwiqA`. I tried submitting it as the flag directly, but that didn't work.

I then used `strings` to search for readable text in the file:

```bash
strings song.mp4
```

Among the output, I found:

```
SGKhwEQIyMzeqKgZwiqAPK
flag.txt
flag.txt
```

The presence of `flag.txt` and the same suspicious string suggested that there might be hidden files embedded in the video.

## Exploitation

I used `foremost`, a file carving tool, to extract any embedded files:

```bash
foremost song.mp4
```

```
Processing: song.mp4
|foundat=flag.txt�
```

This created an output directory with the following structure:

```
output
├── audit.txt
├── mp4
│   └── 00000000.mp4
└── zip
    └── 00095470.zip
```

A hidden zip was found, which I attempted to extract:

```bash
unzip output/zip/00095470.zip
```

```
Archive:  output/zip/00095470.zip
   skipping: flag.txt                need PK compat. v5.1 (can do v4.6)
```

The file required a newer ZIP format. After googling the error, I found that `7z` could handle it:

```bash
7z x output/zip/00095470.zip
```

The tool prompted for a password. I used the suspicious comment from the metadata (`SGKhwEQIyMzeqKgZwiqA`), and it worked. The file extracted successfully and revealed the flag in `flag.txt`.

## Flag

`KDCTF{Anata_wada_whitehacker_des}`
