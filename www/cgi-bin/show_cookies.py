#!/usr/bin/env python3
import os

print("Content-Type: text/html")
print()

cookie_header = os.environ.get('HTTP_COOKIE', '')

print("""
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Your Cookies</title>
    <link rel="stylesheet" href="/styles.css">
</head>
<body>
    <div class="cgi-page">
        <div class="content-wrapper" style="padding-top:100px;">
            <h1>🍪 Your Cookies</h1>
            <p class="subtitle">All cookies sent by your browser</p>
            
            <div style="background:rgba(255,255,255,0.85); padding:40px; border-radius:28px; max-width:800px; margin:40px auto; box-shadow:0 8px 32px rgba(140,180,220,0.25);">
""")

if cookie_header:
    print("<table style='width:100%; border-collapse:collapse;'>")
    print("<tr style='background:rgba(232,244,253,0.5); border-bottom:2px solid rgba(144,200,246,0.3);'>")
    print("<th style='padding:12px; text-align:left; font-weight:700; color:#4a5a74;'>Cookie Name</th>")
    print("<th style='padding:12px; text-align:left; font-weight:700; color:#4a5a74;'>Value</th>")
    print("</tr>")
    
    for cookie in cookie_header.split('; '):
        if '=' in cookie:
            key, value = cookie.split('=', 1)
            print(f"<tr style='border-bottom:1px solid rgba(144,200,246,0.2);'>")
            print(f"<td style='padding:12px; font-weight:600; color:#1e2d4a;'>{key}</td>")
            print(f"<td style='padding:12px; color:#4a5a74; font-family:monospace; font-size:13px;'>{value}</td>")
            print("</tr>")
    
    print("</table>")
else:
    print("<p style='text-align:center; color:#8a97b0; font-size:16px;'>No cookies found 😢</p>")

print("""
            </div>
            <div class="back-section">
                <a href="/cookies.html" class="btn-back">← Back to Cookie Tests</a>
            </div>
        </div>
    </div>
</body>
</html>
""")
