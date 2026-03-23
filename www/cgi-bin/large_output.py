#!/usr/bin/env python3
print("Content-Type: text/html\r")
print("\r")
print("""<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Large Output - Webserv</title>
<link rel="stylesheet" href="/styles.css">
<style>
body { background: linear-gradient(160deg, #e8f4fd 0%, #ede8f5 50%, #f0f4ff 100%); min-height: 100vh; font-family: 'Sora', sans-serif; }
.cgi-result { max-width: 720px; margin: 120px auto 40px; background: rgba(255,255,255,0.82); backdrop-filter: blur(16px); border-radius: 28px; padding: 48px 40px; box-shadow: 0 8px 32px rgba(140,180,220,0.25); border: 1px solid rgba(255,255,255,0.95); }
h1 { font-size: 26px; font-weight: 800; color: #1e2d4a; margin-bottom: 8px; }
.cgi-badge { display: inline-block; background: linear-gradient(135deg, #90c8f6, #a78ec7); color: white; font-size: 11px; font-weight: 700; padding: 4px 14px; border-radius: 50px; margin-bottom: 20px; letter-spacing: 0.8px; text-transform: uppercase; }
.info-bar { background: rgba(232,244,253,0.7); border-radius: 12px; padding: 12px 18px; margin-bottom: 20px; font-size: 13px; color: #4a5a74; border: 1px solid rgba(144,200,246,0.2); }
.line { font-size: 12px; color: #8a97b0; padding: 3px 0; border-bottom: 1px solid rgba(144,200,246,0.1); font-family: monospace; }
.success { background: #dcfce7; color: #15803d; border-radius: 12px; padding: 14px 18px; margin-top: 20px; font-weight: 700; font-size: 14px; }
.back-btn { display: inline-block; margin-top: 28px; padding: 10px 28px; background: white; color: #4a5a74; text-decoration: none; border-radius: 50px; font-weight: 600; font-size: 13px; border: 1.5px solid rgba(144,200,246,0.35); }
</style>
</head>
<body>
<header class="navbar shadow-nav">
  <h2 class="logo2">Webserv</h2>
  <nav><a href="/" class="dark-link">Home</a><a href="/cgi.html" class="dark-link">CGI Tests</a></nav>
</header>
<div class="cgi-result">
  <span class="cgi-badge">GET · Large Output Test</span>
  <h1>📦 Large Output (~100KB)</h1>
  <div class="info-bar">Generating 1000 lines ≈ 100KB of output to test your server's buffer handling.</div>""")

for i in range(1000):
    print(f'<div class="line">Line {i:04d} &nbsp;|&nbsp; {"·" * 60}</div>')

print("""
  <div class="success">✅ Done! Your server handled large CGI output correctly.</div>
  <a href="/cgi.html" class="back-btn">← Back to CGI Tests</a>
</div></body></html>""")