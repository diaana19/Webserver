#!/usr/bin/env python3
import os
import sys
try:
    from urllib.parse import quote, unquote_plus
except ImportError:
    from urllib import quote
    from urlparse import parse_qs

print("Content-Type: text/html")

# Leer POST data
content_length = int(os.environ.get('CONTENT_LENGTH', 0))
post_data = sys.stdin.read(content_length) if content_length > 0 else ""

# Parsear datos
params = {}
for pair in post_data.split('&'):
    if '=' in pair:
        key, value = pair.split('=', 1)
        params[key] = unquote_plus(value)

item = params.get('item', 'Unknown')
quantity = params.get('quantity', '1')

# Leer cookies existentes
cookie_header = os.environ.get('HTTP_COOKIE', '')
cookies = {}
for cookie in cookie_header.split(';'):
    if '=' in cookie:
        key, value = cookie.strip().split('=', 1)
        cookies[key] = value

# Obtener cart actual
cart = cookies.get('cart', '')

# Agregar nuevo item
if cart:
    cart += f",{item}:{quantity}"
else:
    cart = f"{item}:{quantity}"

# URL encode
cart_encoded = quote(cart)

# Establecer cookie
print(f"Set-Cookie: cart={cart_encoded}; Path=/; Max-Age=86400")
print()

print(f"""
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Added to Cart - Webserv</title>
    <link rel="stylesheet" href="/styles.css">
</head>
<body>
    <header class="navbar shadow-nav">
        <h2 class="logo2">Webserv</h2>
        <nav>
            <a href="/" class="dark-link">Home</a>
            <a href="/cookies.html" class="dark-link">Cookie Tests</a>
        </nav>
    </header>

    <div class="cgi-page">
        <div class="content-wrapper" style="max-width:600px; padding-top:100px;">
            <div style="background:rgba(220,252,231,0.8); padding:40px; border-radius:28px; text-align:center; border:2px solid #86efac;">
                <h1 style="color:#15803d; font-size:32px; margin-bottom:16px;">🛒 Added to Cart!</h1>
                <p style="color:#166534; font-size:16px; margin-bottom:16px;">
                    <strong>{item}</strong> × {quantity}<br>
                    has been added to your cart.
                </p>
                <div style="background:#fff; padding:16px; border-radius:12px; margin:16px 0; text-align:left;">
                    <h3 style="margin:0 0 8px 0; color:#15803d;">Your Cart:</h3>
                    <code style="font-size:13px; color:#166534;">{cart}</code>
                </div>
                <a href="/cookies.html" class="btn-back">← Continue Shopping</a>
            </div>
        </div>
    </div>

    <footer><p>Webserv Project | 42 School</p></footer>
</body>
</html>
""")