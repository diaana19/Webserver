#!/bin/bash
echo "Content-Type: text/html"
echo ""

BODY=$(cat)

cat << HTML
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Bash POST - Webserv</title>
<link rel="stylesheet" href="/styles.css">
<style>
body { background: linear-gradient(160deg, #e8f4fd 0%, #ede8f5 50%, #f0f4ff 100%); min-height: 100vh; font-family: 'Sora', sans-serif; }
.cgi-result { max-width: 620px; margin: 120px auto 40px; background: rgba(255,255,255,0.82); backdrop-filter: blur(16px); border-radius: 28px; padding: 48px 40px; box-shadow: 0 8px 32px rgba(140,180,220,0.25); border: 1px solid rgba(255,255,255,0.95); }
h1 { font-size: 26px; font-weight: 800; color: #1e2d4a; margin-bottom: 24px; }
.cgi-badge { display: inline-block; background: linear-gradient(135deg, #90c8f6, #a78ec7); color: white; font-size: 11px; font-weight: 700; padding: 4px 14px; border-radius: 50px; margin-bottom: 20px; letter-spacing: 0.8px; text-transform: uppercase; }
.cgi-row { display: flex; justify-content: space-between; align-items: center; padding: 12px 16px; border-radius: 10px; margin-bottom: 8px; background: rgba(232,244,253,0.5); border: 1px solid rgba(144,200,246,0.2); }
.label { font-size: 12px; font-weight: 700; color: #7bafd4; text-transform: uppercase; }
.value { font-size: 14px; color: #1e2d4a; font-weight: 500; }
.body-box { background: rgba(237,232,245,0.5); border: 1px solid rgba(200,182,226,0.25); border-radius: 12px; padding: 16px 18px; font-family: monospace; font-size: 13px; color: #4a5a74; margin-top: 16px; word-break: break-all; }
.section-lbl { font-size: 12px; font-weight: 700; color: #8a97b0; text-transform: uppercase; letter-spacing: 0.8px; margin: 20px 0 10px; }
.back-btn { display: inline-block; margin-top: 28px; padding: 10px 28px; background: white; color: #4a5a74; text-decoration: none; border-radius: 50px; font-weight: 600; font-size: 13px; border: 1.5px solid rgba(144,200,246,0.35); }
</style>
</head>
<body>
<header class="navbar shadow-nav">
  <h2 class="logo2">Webserv</h2>
  <nav><a href="/" class="dark-link">Home</a><a href="/cgi.html" class="dark-link">CGI Tests</a></nav>
</header>
<div class="cgi-result">
  <span class="cgi-badge">POST · Bash CGI</span>
  <h1>🔧 Bash POST Receiver</h1>
HTML
echo "  <div class=\"cgi-row\"><span class=\"label\">Method</span><span class=\"value\">$REQUEST_METHOD</span></div>"
echo "  <div class=\"cgi-row\"><span class=\"label\">Content-Length</span><span class=\"value\">${CONTENT_LENGTH:-0} bytes</span></div>"
echo "  <p class=\"section-lbl\">Body Received</p>"
echo "  <div class=\"body-box\">$BODY</div>"
cat << HTML
  <a href="/cgi.html" class="back-btn">← Back to CGI Tests</a>
</div>
</body></html>
HTML