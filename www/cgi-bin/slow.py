
#!/usr/bin/env python3
import time
import os
import sys

print("Content-Type: text/html\r")
print("\r")
print("<html><body>")
print(f"<h1>Slow CGI (PID: {os.getpid()})</h1>")
print("<p>Starting 30 second wait...</p>")
sys.stdout.flush()
time.sleep(30)
print("<p style='color:red'>TIMEOUT FAILED - server should have killed this process!</p>")
print("</body></html>")