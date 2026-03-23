#!/usr/bin/env python3
import os

print("Content-Type: text/html")

# Leer todas las cookies actuales
cookie_header = os.environ.get('HTTP_COOKIE', '')
cookies = []

for cookie in cookie_header.split(';'):
    if '=' in cookie:
        key = cookie.strip().split('=', 1)[0]
        cookies.append(key)

# Borrar todas las cookies
for cookie_name in cookies:
    print(f"Set-Cookie: {cookie_name}=; Path=/; Max-Age=0")

print()

cookies_deleted = len(cookies)

print(f"""
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Cookies Cleared - Webserv</title>
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
        <div class="content-wrapper" style="max-width:700px; padding-top:100px;">
            <div style="background:rgba(254,226,226,0.8); padding:40px; border-radius:28px; text-align:center; border:2px solid #fca5a5;">
                <h1 style="color:#b91c1c; font-size:32px; margin-bottom:16px;">🗑️ All Cookies Cleared!</h1>
                <p style="color:#991b1b; font-size:16px; margin-bottom:16px;">
                    <strong>{cookies_deleted} cookie(s)</strong> have been deleted.
                </p>
""")

if cookies:
    print("""
                <div style="background:#fff; padding:16px; border-radius:12px; margin:16px 0; text-align:left;">
                    <h3 style="margin:0 0 8px 0; color:#b91c1c;">Deleted cookies:</h3>
    """)
    for c in cookies:
        print(f"                    <code style='display:block; font-size:13px; margin:4px 0;'>✓ {c}</code>")
    print("                </div>")
else:
    print("""
                <div style="background:#fff; padding:16px; border-radius:12px; margin:16px 0;">
                    <p style='margin:0; color:#991b1b;'>No cookies found to delete</p>
                </div>
    """)

print("""
                <a href="/cgi-bin/show_cookies.py" class="test-btn">View Remaining Cookies</a>
                <a href="/cookies.html" class="btn-back" style="margin-left:12px;">← Back</a>
            </div>
        </div>
    </div>

    <footer><p>Webserv Project | 42 School</p></footer>
</body>
</html>
""")