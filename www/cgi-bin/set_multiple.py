#!/usr/bin/env python3
import os
import sys

print("Content-Type: text/html")

content_length = int(os.environ.get('CONTENT_LENGTH', 0))
post_data = sys.stdin.read(content_length) if content_length > 0 else ""

params = {}
for pair in post_data.split('&'):
    if '=' in pair:
        key, value = pair.split('=', 1)
        params[key] = value.replace('+', ' ').replace('%40', '@')

name = params.get('name', 'Anonymous')
email = params.get('email', 'no-email')
age = params.get('age', '0')

print(f"Set-Cookie: user_name={name}; Path=/; Max-Age=3600")
print(f"Set-Cookie: user_email={email}; Path=/; Max-Age=3600")
print(f"Set-Cookie: user_age={age}; Path=/; Max-Age=3600")
print()

print(f"""
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Cookies Set - Webserv</title>
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
            <div style="background:rgba(254,243,199,0.8); padding:40px; border-radius:28px; text-align:center; border:2px solid #fde047;">
                <h1 style="color:#a16207; font-size:32px; margin-bottom:16px;">✅ 3 Cookies Set!</h1>
                <p style="color:#92400e; font-size:16px; margin-bottom:16px;">
                    The following cookies were created:
                </p>
                <div style="background:#fff; padding:20px; border-radius:12px; text-align:left; margin:16px 0;">
                    <p style="margin:8px 0;"><strong>user_name:</strong> {name}</p>
                    <p style="margin:8px 0;"><strong>user_email:</strong> {email}</p>
                    <p style="margin:8px 0;"><strong>user_age:</strong> {age}</p>
                </div>
                <p style="font-size:14px; color:#92400e; margin-top:12px;">
                    ⏱️ All cookies expire in 1 hour
                </p>
                <a href="/cgi-bin/show_cookies.py" class="test-btn">View All Cookies</a>
                <a href="/cookies.html" class="btn-back" style="margin-left:12px;">← Back</a>
            </div>
        </div>
    </div>

    <footer><p>Webserv Project | 42 School</p></footer>
</body>
</html>
""")