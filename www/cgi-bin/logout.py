#!/usr/bin/env python3

print("Content-Type: text/html")

# Borrar cookies estableciendo Max-Age=0
print("Set-Cookie: session_id=; Path=/; Max-Age=0")
print("Set-Cookie: username=; Path=/; Max-Age=0")
print()

print("""
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Logout - Webserv</title>
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
            <div style="background:rgba(224,242,254,0.8); padding:40px; border-radius:28px; text-align:center; border:2px solid #7dd3fc;">
                <h1 style="color:#0369a1; font-size:32px; margin-bottom:16px;">👋 Logged Out</h1>
                <p style="color:#075985; font-size:16px; margin-bottom:24px;">
                    Your session has been destroyed.<br>
                    All session cookies have been cleared.
                </p>
                <a href="/cookies.html" class="btn-back">← Back to Cookie Tests</a>
            </div>
        </div>
    </div>

    <footer><p>Webserv Project | 42 School</p></footer>
</body>
</html>
""")