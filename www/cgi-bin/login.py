#!/usr/bin/env python3
import os
import sys
import hashlib
import time

print("Content-Type: text/html")

# Leer POST body
content_length = int(os.environ.get('CONTENT_LENGTH', 0))
post_data = sys.stdin.read(content_length) if content_length > 0 else ""

# Parsear datos
params = {}
for pair in post_data.split('&'):
    if '=' in pair:
        key, value = pair.split('=', 1)
        params[key] = value.replace('+', ' ')

username = params.get('username', '')
password = params.get('password', '')

# Validar credenciales (hardcoded para demo)
if username == 'admin' and password == 'secret123':
    # Generar session ID
    session_id = hashlib.md5(f"{username}{time.time()}".encode()).hexdigest()
    
    # Set cookie con session
    print(f"Set-Cookie: session_id={session_id}; Path=/; Max-Age=3600\r")
    print(f"Set-Cookie: username={username}; Path=/; Max-Age=3600\r")
    print("\r")
    
    print("""
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <title>Login Success</title>
        <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
        <div class="cgi-page">
            <div class="content-wrapper" style="max-width:600px; padding-top:100px;">
                <div style="background:rgba(220,252,231,0.8); padding:40px; border-radius:28px; text-align:center; border:2px solid #86efac;">
                    <h1 style="color:#15803d; font-size:32px; margin-bottom:16px;">✅ Login Successful!</h1>
                    <p style="color:#166534; font-size:16px; margin-bottom:24px;">
                        Welcome back, <strong>""" + username + """</strong>!<br>
                        Your session ID: <code style="background:#fff; padding:4px 8px; border-radius:6px;">""" + session_id + """</code>
                    </p>
                    <a href="/cgi-bin/dashboard.py" class="test-btn">Go to Dashboard</a>
                    <a href="/cookies.html" class="btn-back" style="margin-left:12px;">Back to Tests</a>
                </div>
            </div>
        </div>
    </body>
    </html>
    """)
else:
    print()
    print("""
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <title>Login Failed</title>
        <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
        <div class="cgi-page">
            <div class="content-wrapper" style="max-width:600px; padding-top:100px;">
                <div style="background:rgba(252,231,243,0.8); padding:40px; border-radius:28px; text-align:center; border:2px solid #f9a8d4;">
                    <h1 style="color:#be185d; font-size:32px; margin-bottom:16px;">❌ Login Failed</h1>
                    <p style="color:#831843; font-size:16px; margin-bottom:24px;">
                        Invalid username or password.<br>
                        Try: <strong>admin</strong> / <strong>secret123</strong>
                    </p>
                    <a href="/cookies.html" class="btn-back">← Try Again</a>
                </div>
            </div>
        </div>
    </body>
    </html>
    """)