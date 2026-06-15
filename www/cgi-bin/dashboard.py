#!/usr/bin/env python3
import os

print("Content-Type: text/html")
print()

cookie_header = os.environ.get('HTTP_COOKIE', '')
cookies = {}
for cookie in cookie_header.split('; '):
    if '=' in cookie:
        key, value = cookie.split('=', 1)
        cookies[key] = value

session_id = cookies.get('session_id', '')
username = cookies.get('username', 'Guest')

if session_id:
    print(f"""
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <title>Dashboard</title>
        <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
        <header class="navbar shadow-nav">
            <h2 class="logo2">Webserv</h2>
            <nav>
                <a href="/" class="dark-link">Home</a>
                <a href="/cookies.html" class="dark-link">Cookie Tests</a>
                <form method="POST" action="/cgi-bin/logout.py" style="display:inline; margin:0;">
                    <button type="submit" style="padding:7px 16px; font-size:14px; background:linear-gradient(135deg,#f87171,#dc2626);">Logout</button>
                </form>
            </nav>
        </header>
        <div class="cgi-page">
            <div class="content-wrapper" style="padding-top:100px;">
                <h1>👤 Dashboard</h1>
                <p class="subtitle">Welcome back, {username}!</p>
                
                <div style="background:rgba(255,255,255,0.85); padding:40px; border-radius:28px; max-width:700px; margin:40px auto; box-shadow:0 8px 32px rgba(140,180,220,0.25);">
                    <h2 style="margin-bottom:20px; color:#4a5a74;">📊 Your Session Info</h2>
                    <table style="width:100%; border-collapse:collapse;">
                        <tr style="border-bottom:1px solid rgba(144,200,246,0.3);">
                            <td style="padding:12px 0; font-weight:600; color:#4a5a74;">Username:</td>
                            <td style="padding:12px 0; color:#1e2d4a;">{username}</td>
                        </tr>
                        <tr style="border-bottom:1px solid rgba(144,200,246,0.3);">
                            <td style="padding:12px 0; font-weight:600; color:#4a5a74;">Session ID:</td>
                            <td style="padding:12px 0; color:#1e2d4a; font-family:monospace; font-size:13px;">{session_id}</td>
                        </tr>
                        <tr>
                            <td style="padding:12px 0; font-weight:600; color:#4a5a74;">Status:</td>
                            <td style="padding:12px 0; color:#15803d; font-weight:600;">✅ Authenticated</td>
                        </tr>
                    </table>
                    
                    <div style="margin-top:32px; padding:20px; background:rgba(232,244,253,0.7); border-radius:16px; border:1px solid rgba(144,200,246,0.3);">
                        <p style="margin:0; color:#4a5a74; font-size:14px;">
                            🔒 This page is only accessible with a valid session cookie.
                        </p>
                    </div>
                </div>
                
                <div class="back-section">
                    <a href="/cookies.html" class="btn-back">← Back to Cookie Tests</a>
                </div>
            </div>
        </div>
    </body>
    </html>
    """)
else:
    # No autenticado
    print("""
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <title>Access Denied</title>
        <link rel="stylesheet" href="/styles.css">
    </head>
    <body>
        <div class="cgi-page">
            <div class="content-wrapper" style="max-width:600px; padding-top:100px;">
                <div style="background:rgba(252,231,243,0.8); padding:40px; border-radius:28px; text-align:center; border:2px solid #f9a8d4;">
                    <h1 style="color:#be185d; font-size:32px; margin-bottom:16px;">🔒 Access Denied</h1>
                    <p style="color:#831843; font-size:16px; margin-bottom:24px;">
                        You must be logged in to view this page.
                    </p>
                    <a href="/cookies.html" class="test-btn">← Login First</a>
                </div>
            </div>
        </div>
    </body>
    </html>
    """)