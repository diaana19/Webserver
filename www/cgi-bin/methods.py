#!/usr/bin/env python3
import os, sys

method = os.getenv('REQUEST_METHOD', 'none').upper()
content_length = int(os.getenv('CONTENT_LENGTH', 0))

print("Content-Type: text/html\r")
print("\r")

badge_color = "#90c8f6, #a78ec7" if method == "GET" else "#b8e0d2, #7bafd4"

print(f"""<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Methods Handler - Webserv</title>
<link rel="stylesheet" href="/styles.css">
<style>
body {{ background: linear-gradient(160deg, #e8f4fd 0%, #ede8f5 50%, #f0f4ff 100%); min-height: 100vh; font-family: 'Sora', sans-serif; }}
.cgi-result {{ max-width: 620px; margin: 120px auto 40px; background: rgba(255,255,255,0.82); backdrop-filter: blur(16px); border-radius: 28px; padding: 48px 40px; box-shadow: 0 8px 32px rgba(140,180,220,0.25); border: 1px solid rgba(255,255,255,0.95); }}
h1 {{ font-size: 26px; font-weight: 800; color: #1e2d4a; margin-bottom: 24px; }}
.cgi-badge {{ display: inline-block; background: linear-gradient(135deg, {badge_color}); color: white; font-size: 11px; font-weight: 700; padding: 4px 14px; border-radius: 50px; margin-bottom: 20px; letter-spacing: 0.8px; text-transform: uppercase; }}
.method-pill {{ display: inline-block; font-size: 22px; font-weight: 800; padding: 8px 28px; border-radius: 50px; margin-bottom: 24px; background: linear-gradient(135deg, {badge_color}); color: white; letter-spacing: 1px; }}
.cgi-row {{ display: flex; justify-content: space-between; padding: 12px 16px; border-radius: 10px; margin-bottom: 8px; background: rgba(232,244,253,0.5); border: 1px solid rgba(144,200,246,0.2); }}
.label {{ font-size: 12px; font-weight: 700; color: #7bafd4; text-transform: uppercase; }}
.value {{ font-size: 14px; color: #1e2d4a; font-weight: 500; }}
.param-row {{ display: flex; justify-content: space-between; padding: 12px 16px; border-radius: 10px; margin-bottom: 8px; background: rgba(237,232,245,0.5); border: 1px solid rgba(200,182,226,0.2); }}
.param-key {{ font-size: 13px; font-weight: 700; color: #a78ec7; }}
.param-val {{ font-size: 14px; color: #1e2d4a; font-weight: 600; }}
.section-lbl {{ font-size: 12px; font-weight: 700; color: #8a97b0; text-transform: uppercase; letter-spacing: 0.8px; margin: 20px 0 10px; }}
.back-btn {{ display: inline-block; margin-top: 28px; padding: 10px 28px; background: white; color: #4a5a74; text-decoration: none; border-radius: 50px; font-weight: 600; font-size: 13px; border: 1.5px solid rgba(144,200,246,0.35); }}
</style>
</head>
<body>
<header class="navbar shadow-nav">
  <h2 class="logo2">Webserv</h2>
  <nav><a href="/" class="dark-link">Home</a><a href="/cgi.html" class="dark-link">CGI Tests</a></nav>
</header>
<div class="cgi-result">
  <span class="cgi-badge">{method} · Methods Handler</span>
  <h1>⚡ GET + POST Handler</h1>
  <div class="method-pill">{method}</div>""")

if method == 'GET':
    query = os.getenv('QUERY_STRING', '')
    print('<p class="section-lbl">GET Parameters</p>')
    if query:
        for p in query.split('&'):
            if '=' in p:
                k, v = p.split('=', 1)
                print(f'<div class="param-row"><span class="param-key">{k}</span><span class="param-val">{v}</span></div>')
    else:
        print('<div class="cgi-row"><span class="value">No parameters. Try ?key=value</span></div>')
elif method == 'POST':
    body = sys.stdin.read(content_length) if content_length > 0 else ''
    print('<p class="section-lbl">POST Body</p>')
    if body:
        for p in body.split('&'):
            if '=' in p:
                k, v = p.split('=', 1)
                print(f'<div class="param-row"><span class="param-key">{k}</span><span class="param-val">{v}</span></div>')

print('<a href="/cgi.html" class="back-btn">← Back to CGI Tests</a></div></body></html>')