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
        params[key] = value

theme = params.get('theme', 'light')

print(f"Set-Cookie: theme={theme}; Path=/; Max-Age=2592000")
print()

theme_config = {
    'light': {'icon': '☀️', 'name': 'Light Mode', 'color': '#f59e0b'},
    'dark': {'icon': '🌙', 'name': 'Dark Mode', 'color': '#6366f1'},
    'blue': {'icon': '💙', 'name': 'Blue Theme', 'color': '#0ea5e9'}
}

config = theme_config.get(theme, theme_config['light'])

print(f"""
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Theme Saved - Webserv</title>
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
            <div style="background:rgba(233,213,255,0.8); padding:40px; border-radius:28px; text-align:center; border:2px solid #c084fc;">
                <h1 style="color:#7e22ce; font-size:32px; margin-bottom:16px;">{config['icon']} Theme Saved!</h1>
                <p style="color:#6b21a8; font-size:16px; margin-bottom:24px;">
                    Your preference has been saved: <strong>{config['name']}</strong><br>
                    Cookie expires in 30 days.
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