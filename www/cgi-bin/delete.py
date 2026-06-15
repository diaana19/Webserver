# delete_file.py
import http.client

HOST = "127.0.0.1"
PORT = 8080

def delete_file(path):
    conn = http.client.HTTPConnection(HOST, PORT)
    
    conn.request("DELETE", path)
    response = conn.getresponse()
    
    print("Status:", response.status)
    print("Reason:", response.reason)
    print("Response:", response.read().decode())

    conn.close()

if __name__ == "__main__":
    file_to_delete = input("Enter file path (e.g. /test.txt): ")
    delete_file(file_to_delete)