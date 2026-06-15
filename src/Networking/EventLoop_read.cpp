#include "EventLoop.hpp"
#include "CookieParser.hpp"

void EventLoop::getMessageInFd(int fd) {
    char buffer[8192];
    int n = recv(fd, buffer, sizeof(buffer), 0);
    
    if (n <= 0) {
        if (n == 0) {
            std::cout << "Client disconnected (EOF)" << std::endl;
            removeClient(fd);
        } else if (n == -1) {
            removeClient(fd);
        }
        return;
    }
    RequestState& request = activeRequests[fd];
    request.lastActive = std::time(NULL);
    request.getBuffer().append(buffer, n);
    if (!request.getHasFinishReadHeader()) {
        size_t header_end = request.getBuffer().find("\r\n\r\n");
        if (header_end != std::string::npos) {
            request.setHeader(request.getBuffer().substr(0, header_end + 4));
            request.setBody(request.getBuffer().substr(header_end + 4));
			HttpRequest httpRequest = parseHeader(fd, request);
			
			if(httpRequest.method.empty())
				return;
			request.setRequestHttp(httpRequest); 
            request.setHasFinishReadHeader(true);
            request.setStatus(HAS_FINISH_PARSEHEADER);
        }
    } else {
        request.getBody().append(buffer, n);
    }
};

HttpRequest EventLoop::parseHeader(int fd, RequestState &currRequestState) {
	
	HttpRequest httpRequest;
	
	Client* client = _clientManager->getClient(fd);
	if (client != NULL)
		httpRequest.clientIP = client->getIp();
		
	HeadersParsing headersParser;
	headersParser.parse(currRequestState.getHeader(), httpRequest);
	
	RequestLine lineParser;
	std::string firstLine = getFirstLine(currRequestState.getHeader());
	if (!lineParser.parse(firstLine, httpRequest)){
		std::cout << "❌ FAIL" << std::endl;
		
		HttpResponse errorResponse;
		errorResponse.statusCode = 400;

		
		ServerConfig serverConfig = findTheServerConfig(httpRequest);
        httpRequest.serverConfig = serverConfig;
        currRequestState.setRequestHttp(httpRequest);
        currRequestState.setHttpResponse(errorResponse);
        currRequestState.setStatus(CAN_BUILD_RESPONSE);
        turnToEventWrite(fd);
    	return httpRequest;
	}
    return httpRequest;
};

ServerConfig EventLoop::findTheServerConfig(HttpRequest &httpRequest) {
	ServerConfig serverConfig;
	std::map<std::string, std::string>::iterator hostIt = httpRequest.headers.find("Host");
	if (hostIt == httpRequest.headers.end())
		return (serverConfig);
	std::pair<std::string, int> ipAndPort = getHostAndPort(hostIt->second);
	int serverIndex = searchIndexServer(ipAndPort.first, ipAndPort.second);
	if (serverIndex != -1)
		serverConfig = this->allServersConfig[serverIndex];
	return (serverConfig);
}

void logSocketData(const std::string &data)
{
    std::ofstream log("socket_debug.log", std::ios::app | std::ios::binary);

    if (!log)
    {
        std::cerr << "Failed to open debug log\n";
        return;
    }
    log.write(data.data(), data.size());
}

size_t EventLoop::hex_to_decimal(const std::string& hex)
{
    return std::strtoul(hex.c_str(), NULL, 16);
};

void resizeBody(std::string &body, size_t lastRead) {
    body = body.substr(lastRead);
};

std::string EventLoop::readChunked(std::string &body, RequestState &reqState, size_t &consumed) {
    (void)reqState;
    size_t posSizeHexa = body.find("\r\n");
    if (posSizeHexa == std::string::npos) {
        consumed = 0;
        return ("");
    }
    std::string sizeHex = body.substr(0, posSizeHexa);
    if (sizeHex == "0") {
        consumed = std::string::npos;
        return ("");
    }
    
    size_t chunkSize = hex_to_decimal(sizeHex);
    size_t dataStart = posSizeHexa + 2;
    if (body.size() < dataStart + chunkSize + 2) {
        consumed = 0;
        return ("");
    }
    std::string line = body.substr(dataStart, chunkSize);
    consumed = dataStart + chunkSize + 2;
    
    return (line);
}

void EventLoop::parseChunked(std::string &wholeBody, RequestState &reqState) {
    while (1) {
        size_t consumed = 0;
        std::string chunk = readChunked(wholeBody, reqState, consumed);
        if (consumed == 0) {
            
            return ;
        }
        if (consumed == std::string::npos) {
            reqState.setReadLengthChunked(std::string::npos);
            
            
            return ;
        }
        wholeBody = wholeBody.substr(consumed);
        reqState.setBufferChunked(reqState.getBufferChunked() + chunk);
        
    }
}

bool has_expect_100_continue(const std::string& headers) {
    size_t pos = headers.find("Expect:");
    if (pos == std::string::npos) {
        pos = headers.find("expect:");
    }
    if (pos != std::string::npos) {
        size_t end_line = headers.find("\r\n", pos);
        std::string expect_line = headers.substr(pos, end_line - pos);
        if (expect_line.find("100-continue") != std::string::npos)
            return (true);
    }
    return (false);
}

void EventLoop::receiveChunkedData(int fd, RequestState & currRequestState) {
    BodyParser bodyParser;
    HttpRequest& httpRequest = currRequestState.getHttpRequest();

    bool expect_continue = has_expect_100_continue(currRequestState.getHeader());
    if (expect_continue && currRequestState.getBody().size() == 0) {
        currRequestState.response = "HTTP/1.1 100 Continue\r\n\r\n";
        HttpResponse httpResponse;
        httpResponse.statusCode = 100;
        currRequestState.setHttpResponse(httpResponse);
        currRequestState.setStatus(CAN_SEND_MESSAGE);
        turnToEventWrite(fd);
        return ;
    }
    if (currRequestState.getBody().find("\r\n\r\n") == std::string::npos)
        return ;
    
    parseChunked(currRequestState.getBody(), currRequestState);
    if (currRequestState.getReadLengthChunked() == std::string::npos) {
        currRequestState.setBody(currRequestState.getBufferChunked());
	    bodyParser.parseBody(currRequestState.getBody(), httpRequest);
        currRequestState.setStatus(HAS_FINISH_PARSEBODY);  
    }
}

void EventLoop::parseWholeReq(int fd, RequestState &currRequestState){
    HttpRequest& httpRequest = currRequestState.getHttpRequest();
        
    std::map<std::string, std::string>::iterator it = httpRequest.headers.find("Content-Length");
    std::map<std::string, std::string>::iterator itEncoding = httpRequest.headers.find("Transfer-Encoding");
    if (it != httpRequest.headers.end())
	{
		size_t contentLength = std::strtoul(it->second.c_str(), NULL, 10);
		if (currRequestState.getBody().size() >= contentLength)
		{
			ServerConfig serverConfig = findTheServerConfig(httpRequest);
            httpRequest.serverConfig = serverConfig;
            
            
			if (serverConfig.clientMaxBodySize > 0 && (contentLength > serverConfig.clientMaxBodySize 
                || currRequestState.getBody().size() > contentLength))
			{
				HttpResponse httpResponse;
				httpResponse.statusCode = 413;
				std::string responseStr = constructResponse(httpResponse, serverConfig);
                currRequestState.setHttpResponse(httpResponse);
                currRequestState.setStatus(CAN_BUILD_RESPONSE);
                turnToEventWrite(fd);
				return ;
			}
			
			BodyParser bodyParser;
			bodyParser.parseBody(currRequestState.getBody(), httpRequest);
			currRequestState.setStatus(HAS_FINISH_PARSEBODY);
		}
		else if (currRequestState.getBody().size() <= contentLength)
			return ;
	}
    else if (itEncoding != httpRequest.headers.end()) {
        if (itEncoding->second.find("chunked") != std::string::npos) {
            receiveChunkedData(fd, currRequestState);
        }
    }
    else
    {
		currRequestState.setRequestHttp(httpRequest);
        currRequestState.setStatus(HAS_FINISH_PARSEBODY);
    }
};

void EventLoop::handleRead(int fd) {
    std::map<int, RequestState>::iterator it = activeRequests.find(fd);
    if (it == activeRequests.end()) {
        
        RequestState newReqState;
        this->activeRequests[fd] = newReqState;
    }
        
    
    if (activeRequests[fd].getStatus() == STILL_GETTING_MSG) {
		
        getMessageInFd(fd); 
    }
    if (activeRequests[fd].getStatus() == HAS_FINISH_PARSEHEADER) {
		
        parseWholeReq(fd, activeRequests[fd]);
    }
    if (activeRequests[fd].getStatus() == HAS_FINISH_PARSEBODY) {
		
        processRequest(fd);
    }
};