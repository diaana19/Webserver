# Webserv: A C++98 HTTP/1.1 Server

![C++](https://img.shields.io/badge/C++-98-blue.svg)
![HTTP](https://img.shields.io/badge/HTTP-1.1-green.svg)
![42](https://img.shields.io/badge/42-Project-orange.svg)

This project has been created as part of the 42 curriculum by **dirituay**, **vali**, and **rmarcas-**.

---

## 📖 Description

**Webserv** is a custom-built HTTP/1.1 web server developed in **C++98**. Inspired by the performance and architecture of NGINX, this server is designed to be fully **non-blocking** and to handle multiple client connections concurrently using **I/O multiplexing** (select/poll/epoll/kqueue).

The goal of this project is to understand the low-level intricacies of the **Hypertext Transfer Protocol (HTTP)** and how a server manages sockets, processes request headers, and serves both static and dynamic content through **CGI (Common Gateway Interface)**.

---

## 🚀 Instructions for Use

### Compilation

The project utilizes a **Makefile** for a streamlined build process. To generate the `webserv` binary, execute the following command in the project root:

```bash
make
```

### Execution

The server requires a configuration file to define its behavior.


```bash
# Launch with a specific configuration

./webserv ./config/default.conf
```

### Interaction

Once initialized, the server listens for incoming connections. You can interact with it via a standard web browser or command-line utilities:

```bash
# Testing a GET request with curl
curl -v http://localhost:8080

# Testing a POST file upload
curl -X POST -H "Content-Type: text/plain" --data "Sample Data" http://localhost:8080/upload

# Testing file upload
curl -v -F "file=@test.txt" http://localhost:8080/upload

# Testing DELETE method
curl -v -X DELETE http://localhost:8080/file.txt

# Viewing response headers only
curl -I http://localhost:8080/
```

---

## ✨ Key Features

### HTTP/1.1 Compliance
- Support for mandatory methods: **GET**, **POST**, **DELETE**
- RFC-compliant status codes (200, 404, 500, etc.)
- Proper handling of HTTP headers and message formatting

### I/O Multiplexing
- Utilization of **epoll()** (Linux) or **select()** to monitor all file descriptors within a single process
- Efficient concurrent client handling without threading

### Non-blocking Architecture
- Enforcement of non-blocking I/O across all sockets using `fcntl(fd, F_SETFL, O_NONBLOCK)`
- Server remains responsive under high load

### Virtual Hosting
- Support for multiple server blocks sharing the same IP and port
- Distinguished via the **Host** header (RFC 7230, Section 5.4)

### Dynamic Content (CGI)
- CGI integration for executing external scripts (Python, PHP, Bash, etc.)
- Timeout management to prevent hanging scripts
- Environment variable passing (REQUEST_METHOD, QUERY_STRING, etc.)

### Static Content
- Efficient delivery of static files (HTML, CSS, JS, images)
- Automatic MIME type detection
- Directory listing support (**autoindex**)

### Advanced Configuration
- Custom error pages
- Client body size limits to prevent payload abuse
- URL redirection (301/302 status codes)
- Configurable routes with method restrictions

---

## 🔧 Configuration Example

```nginx
server 
{
    listen 8080;
    host 127.0.0.1;
    server_name webserv;
    root www;     
    index index.html;    
    client_max_body_size 10485760;
    
    error_page 404 www/errors/404.html;
    
    location / 
	{
        root www;
        index index.html;
        allow_methods GET POST DELETE;
        autoindex off;
    }
    
    location /contact   
	{
        allow_methods GET POST;
    }
    
    location /upload
	{
        allow_methods GET POST DELETE;
        upload_path www/upload/;
        autoindex on;
    }

    location /cgi-bin 
    {
        allow_methods GET POST;
        cgi_path www/cgi-bin;
        cgi_interpreter /usr/bin/python3;
        cgi_ext .php .py .sh;
    }

    location /redirect 
    {
		return 302 /team.txt;
	}
}
```

---

## 🧪 Testing Tools

### Basic Testing
```bash
# Simple GET request
curl http://localhost:8080/

# Verbose output with headers
curl -v http://localhost:8080/index.html

# Follow redirects
curl -L http://localhost:8080/old-page
```

### POST Testing
```bash
# Simple POST with data
curl -X POST -d "key=value" http://localhost:8080/form

# POST JSON
curl -X POST \
  -H "Content-Type: application/json" \
  -d '{"name":"test","value":42}' \
  http://localhost:8080/api

# Upload file
curl -F "file=@document.pdf" http://localhost:8080/upload
```

### CGI Testing
```bash
# Test Python CGI
curl http://localhost:8080/cgi-bin/test.py?name=John

# Test with POST body
curl -X POST \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "username=admin&password=secret" \
  http://localhost:8080/cgi-bin/login.py
```

### Load Testing
```bash
# Install siege
sudo apt install siege

# Run load test (10 concurrent users for 30 seconds)
siege -c 10 -t 30S http://localhost:8080/

# Benchmark
ab -n 1000 -c 10 http://localhost:8080/
```

### Memory Leak Testing
```bash
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-fds=yes \
         ./webserv ./config/default.conf
```

---

## 🎓 Evaluation Preparation (Technical Q&A)

### Multiplexing Logic
The heart of the server is a main event loop centered on a single `poll()` (or `select()`) call. This call monitors a dynamically updated collection of file descriptors—typically a `std::vector<pollfd>`—for both **READ** and **WRITE** events simultaneously. To ensure fairness and prevent resource monopolization, the server strictly performs only **one read** or **one write** operation per client per loop iteration.

### Non-blocking Sockets and Resource Management
To prevent the server from hanging on a `recv` or `send` call, every socket must be set to **non-blocking mode** using:
```cpp
fcntl(fd, F_SETFL, O_NONBLOCK);
```
This requirement applies to both the initial listening sockets and every accepted client socket. If an operation would block, the call returns immediately with `EAGAIN` or `EWOULDBLOCK`, allowing the server to continue processing other clients in the multiplexing loop.

### Handling Multiple Ports and Virtual Hosting
The server binds a unique listening socket to every unique port defined in the configuration. When multiple server blocks share the same IP and port, the server relies on the **Host header** (as mandated by RFC 7230, Section 5.4) as the sole mechanism to distinguish which virtual server should handle the request. This allows for sophisticated multihomed hosting environments.

### Error Handling on I/O
The server meticulously monitors the return values of all I/O system calls:
- A return value of **0** indicates the connection was closed by the client
- A return value of **-1** indicates a system error

### CGI Execution and Process Control
Dynamic requests trigger a `fork()` and `execve()` sequence. The server populates the child process's environment with standard CGI variables:
- `REQUEST_METHOD`
- `QUERY_STRING`
- `CONTENT_TYPE`
- `CONTENT_LENGTH`
- `PATH_INFO`

Communication occurs via **pipes**. To prevent zombie processes and server hangs, the parent process monitors the CGI child using `waitpid()` with the `WNOHANG` flag and implements a timer to terminate scripts that exceed a defined timeout.

---

## 📚 Resources and References

- **[Beej's Guide to Network Programming]**
  - The definitive guide for Unix socket programming


- **[MDN Web Docs - HTTP]** - Modern HTTP reference
  - HTTP headers documentation
  - MIME types reference
  - Status code examples

- **[NGINX Documentation]** - Configuration reference
  - Configuration file syntax
  - Directive documentation
  - Best practices

- **[HTTP Made Really Easy]** - Beginner-friendly HTTP tutorial
  - Basic HTTP concepts
  - Request/response examples
  - Protocol overview

### Community Resources
- **[Stack Overflow]** - Problem-solving and debugging
  - Tags: `[c++]`, `[sockets]`, `[select]`, `[epoll]`, `[fork]`, `[pipe]`
  - Non-blocking I/O patterns
  - Common pitfalls and solutions

---

## 🤖 AI Usage Disclosure

In accordance with 42 project directives, **Artificial Intelligence (LLMs)** was used to assist in:
- Refining technical documentation to meet professional standards
- Code review and optimization suggestions

---


## 📝 License

This project is part of the 42 school curriculum and follows the school's academic integrity policies.

---


**Made with ❤️ at 42**
