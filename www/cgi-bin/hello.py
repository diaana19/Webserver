#!/usr/bin/env python3
import os
print("Content-Type: text/html\r")
print("\r")
print("""<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Hello Python - Webserv</title>
<link rel="stylesheet" href="/styles.css">
<style>
.cgi-result { max-width: 600px; margin: 120px auto 40px; background: rgba(255,255,255,0.82); backdrop-filter: blur(16px); border-radius: 28px; padding: 48px 40px; box-shadow: 0 8px 32px rgba(140,180,220,0.25); border: 1px solid rgba(255,255,255,0.95); font-family: 'Sora', sans-serif; }
.cgi-result h1 { font-size: 28px; font-weight: 800; color: #1e2d4a; margin-bottom: 24px; letter-spacing: -0.5px; }
.cgi-badge { display: inline-block; background: linear-gradient(135deg, #90c8f6, #a78ec7); color: white; font-size: 11px; font-weight: 700; padding: 4px 14px; border-radius: 50px; margin-bottom: 20px; letter-spacing: 0.8px; text-transform: uppercase; }
.cgi-row { display: flex; justify-content: space-between; align-items: center; padding: 12px 16px; border-radius: 10px; margin-bottom: 8px; background: rgba(232,244,253,0.5); border: 1px solid rgba(144,200,246,0.2); }
.cgi-row .label { font-size: 12px; font-weight: 700; color: #7bafd4; text-transform: uppercase; letter-spacing: 0.5px; }
.cgi-row .value { font-size: 14px; color: #1e2d4a; font-weight: 500; }
.back-btn { display: inline-block; margin-top: 28px; padding: 10px 28px; background: white; color: #4a5a74; text-decoration: none; border-radius: 50px; font-weight: 600; font-size: 13px; border: 1.5px solid rgba(144,200,246,0.35); transition: all 0.2s; }
.back-btn:hover { background: #e8f4fd; color: #7bafd4; }
body { background: linear-gradient(160deg, #e8f4fd 0%, #ede8f5 50%, #f0f4ff 100%); min-height: 100vh; }
</style>
</head>
<body>
<header class="navbar shadow-nav">
  <h2 class="logo2">Webserv</h2>
  <nav><a href="/" class="dark-link">Home</a><a href="/cgi.html" class="dark-link">CGI Tests</a></nav>
</header>
<div class="cgi-result">
  <span class="cgi-badge">GET · Python CGI</span>
  <h1>👋 Hello from Python!</h1>""")

method  = os.getenv('REQUEST_METHOD', 'NOT SET')
server  = os.getenv('SERVER_NAME', 'NOT SET')
port    = os.getenv('SERVER_PORT', 'NOT SET')
script  = os.getenv('SCRIPT_NAME', 'NOT SET')

print(f"""
  <div class="cgi-row"><span class="label">Method</span><span class="value">{method}</span></div>
  <div class="cgi-row"><span class="label">Server</span><span class="value">{server}:{port}</span></div>
  <div class="cgi-row"><span class="label">Script</span><span class="value">{script}</span></div>
  <div class="cgi-row"><span class="label">Status</span><span class="value" style="color:#15803d;font-weight:700;">✅ CGI is working!</span></div>
  <a href="/cgi.html" class="back-btn">← Back to CGI Tests</a>
</div>
</body></html>""")