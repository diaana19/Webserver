#!/usr/bin/php
<?php
$method = getenv('REQUEST_METHOD') ?: 'NOT SET';
$query  = getenv('QUERY_STRING')   ?: '';
$server = getenv('SERVER_NAME')    ?: 'NOT SET';
$port   = getenv('SERVER_PORT')    ?: 'NOT SET';

$params = [];
if ($query) {
    foreach (explode('&', $query) as $p) {
        if (strpos($p, '=') !== false) {
            list($k, $v) = explode('=', $p, 2);
            $params[$k] = $v;
        }
    }
}

echo "Content-Type: text/html\r\n";
echo "\r\n";
?>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Hello PHP - Webserv</title>
<link rel="stylesheet" href="/styles.css">
<style>
body { background: linear-gradient(160deg, #e8f4fd 0%, #ede8f5 50%, #f0f4ff 100%); min-height: 100vh; font-family: 'Sora', sans-serif; }
.cgi-result { max-width: 620px; margin: 120px auto 40px; background: rgba(255,255,255,0.82); backdrop-filter: blur(16px); border-radius: 28px; padding: 48px 40px; box-shadow: 0 8px 32px rgba(140,180,220,0.25); border: 1px solid rgba(255,255,255,0.95); }
h1 { font-size: 26px; font-weight: 800; color: #1e2d4a; margin-bottom: 24px; }
.cgi-badge { display: inline-block; background: linear-gradient(135deg, #90c8f6, #a78ec7); color: white; font-size: 11px; font-weight: 700; padding: 4px 14px; border-radius: 50px; margin-bottom: 20px; letter-spacing: 0.8px; text-transform: uppercase; }
.cgi-row { display: flex; justify-content: space-between; align-items: center; padding: 12px 16px; border-radius: 10px; margin-bottom: 8px; background: rgba(232,244,253,0.5); border: 1px solid rgba(144,200,246,0.2); }
.label { font-size: 12px; font-weight: 700; color: #7bafd4; text-transform: uppercase; letter-spacing: 0.5px; }
.value { font-size: 14px; color: #1e2d4a; font-weight: 500; }
.param-row { display: flex; justify-content: space-between; padding: 12px 16px; border-radius: 10px; margin-bottom: 8px; background: rgba(237,232,245,0.5); border: 1px solid rgba(200,182,226,0.2); }
.param-key { font-size: 13px; font-weight: 700; color: #a78ec7; }
.param-val { font-size: 14px; color: #1e2d4a; font-weight: 600; }
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
  <span class="cgi-badge">GET · PHP CGI</span>
  <h1>🐘 Hello from PHP!</h1>
  <div class="cgi-row"><span class="label">Method</span><span class="value"><?php echo $method; ?></span></div>
  <div class="cgi-row"><span class="label">Server</span><span class="value"><?php echo "$server:$port"; ?></span></div>
  <div class="cgi-row"><span class="label">Status</span><span class="value" style="color:#15803d;font-weight:700;">✅ PHP CGI is working!</span></div>
<?php if (!empty($params)): ?>
  <p class="section-lbl">Query Parameters</p>
  <?php foreach ($params as $k => $v): ?>
  <div class="param-row"><span class="param-key"><?php echo htmlspecialchars($k); ?></span><span class="param-val"><?php echo htmlspecialchars($v); ?></span></div>
  <?php endforeach; ?>
<?php endif; ?>
  <a href="/cgi.html" class="back-btn">← Back to CGI Tests</a>
</div>
</body></html>