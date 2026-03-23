#!/usr/bin/env python3
import time

print("Content-Type: text/html")

# Cookie que dura 30 días
timestamp = int(time.time())
print(f"Set-Cookie: persistent_id={timestamp}; Path=/; Max-Age=2592000")
print()

print(f"""
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Persistent Cookie - Webserv</title>
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
            <div style="background:rgba(219,234,254,0.8); padding:40px; border-radius:28px; text-align:center; border:2px solid #60a5fa;">
                <h1 style="color:#1e40af; font-size:32px; margin-bottom:16px;">📌 Persistent Cookie Set!</h1>
                <p style="color:#1e3a8a; font-size:16px; margin-bottom:16px;">
                    A long-lived cookie has been created.<br>
                    It will persist for <strong>30 days</strong>.
                </p>
                <div style="background:#fff; padding:16px; border-radius:12px; margin:16px 0;">
                    <p style="margin:0; color:#1e40af;">
                        <strong>Cookie ID:</strong> <code>{timestamp}</code>
                    </p>
                </div>
                <p style="font-size:14px; color:#1e3a8a;">
                    ⏳ Expires: 30 days from now<br>
                    Even if you close your browser, this cookie will remain.
                </p>
                <a href="/cgi-bin/show_cookies.py" class="test-btn">View Cookies</a>
                <a href="/cookies.html" class="btn-back" style="margin-left:12px;">← Back</a>
            </div>
        </div>
    </div>

    <footer><p>Webserv Project | 42 School</p></footer>
</body>
</html>
""")