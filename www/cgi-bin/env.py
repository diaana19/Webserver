#!/usr/bin/env python3
import os
print("Content-Type: text/html\r")
print("\r")
print("""<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Env Variables - Webserv</title>
<link rel="stylesheet" href="/styles.css">
<style>
body { background: linear-gradient(160deg, #e8f4fd 0%, #ede8f5 50%, #f0f4ff 100%); min-height: 100vh; font-family: 'Sora', sans-serif; }
.cgi-result { max-width: 720px; margin: 120px auto 40px; background: rgba(255,255,255,0.82); backdrop-filter: blur(16px); border-radius: 28px; padding: 48px 40px; box-shadow: 0 8px 32px rgba(140,180,220,0.25); border: 1px solid rgba(255,255,255,0.95); }
h1 { font-size: 26px; font-weight: 800; color: #1e2d4a; margin-bottom: 24px; letter-spacing: -0.5px; }
.cgi-badge { display: inline-block; background: linear-gradient(135deg, #90c8f6, #a78ec7); color: white; font-size: 11px; font-weight: 700; padding: 4px 14px; border-radius: 50px; margin-bottom: 20px; letter-spacing: 0.8px; text-transform: uppercase; }
.env-row { display: grid; grid-template-columns: 220px 1fr 80px; align-items: center; padding: 10px 14px; border-radius: 10px; margin-bottom: 6px; background: rgba(232,244,253,0.45); border: 1px solid rgba(144,200,246,0.18); }
.env-row .var { font-size: 12px; font-weight: 700; color: #7bafd4; }
.env-row .val { font-size: 13px; color: #1e2d4a; word-break: break-all; }
.ok  { font-size: 11px; font-weight: 700; color: #15803d; background: #dcfce7; padding: 2px 10px; border-radius: 50px; text-align: center; }
.nok { font-size: 11px; font-weight: 700; color: #be185d; background: #fce7f3; padding: 2px 10px; border-radius: 50px; text-align: center; }
.back-btn { display: inline-block; margin-top: 28px; padding: 10px 28px; background: white; color: #4a5a74; text-decoration: none; border-radius: 50px; font-weight: 600; font-size: 13px; border: 1.5px solid rgba(144,200,246,0.35); }
.back-btn:hover { background: #e8f4fd; color: #7bafd4; }
</style>
</head>
<body>
<header class="navbar shadow-nav">
  <h2 class="logo2">Webserv</h2>
  <nav><a href="/" class="dark-link">Home</a><a href="/cgi.html" class="dark-link">CGI Tests</a></nav>
</header>
<div class="cgi-result">
  <span class="cgi-badge">GET · Environment Variables</span>
  <h1>🔍 CGI Environment Variables</h1>""")

required_vars = [
    'GATEWAY_INTERFACE','SERVER_PROTOCOL','SERVER_SOFTWARE',
    'SERVER_NAME','SERVER_PORT','REQUEST_METHOD',
    'PATH_INFO','PATH_TRANSLATED','SCRIPT_NAME',
    'QUERY_STRING','REMOTE_HOST','REMOTE_ADDR',
    'CONTENT_TYPE','CONTENT_LENGTH',
]

for var in required_vars:
    value = os.getenv(var, '')
    if value:
        badge = '<span class="ok">OK</span>'
        display = value
    else:
        badge = '<span class="nok">EMPTY</span>'
        display = '—'
    print(f'<div class="env-row"><span class="var">{var}</span><span class="val">{display}</span>{badge}</div>')

print("""
  <a href="/cgi.html" class="back-btn">← Back to CGI Tests</a>
</div></body></html>""")