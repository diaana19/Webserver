#include "EventLoop.hpp"

std::string EventLoop::constructResponse(HttpResponse &httpResponse, const ServerConfig &serverConfig){
	if (httpResponse.body.empty()) {
		ErrorHandler errorHandler;
		httpResponse.body = errorHandler.generate(httpResponse.statusCode, serverConfig);
	}

	StatusBuilder statusBuilder;
	HeadersBuilder headersBuillder;

	std::string response;
	response += statusBuilder.build(httpResponse.statusCode);
	response += headersBuillder.build(httpResponse);
	response += httpResponse.body;
	return (response);
};

ssize_t EventLoop::sendMessage(int fd, std::string &response) {
	ssize_t sent = send(fd, response.c_str(), response.length(), 0);
	
	std::cout << GREEN << "Send message to client fd: " << RESET << fd << std::endl;
	return (sent);
};

void EventLoop::handleWrite(int fd) {
	RequestState& reqState = this->activeRequests[fd];
	printHeaders(reqState.getHttpRequest());
	if (reqState.getStatus() == CAN_BUILD_RESPONSE) {
		HttpRequest& httpReq = reqState.getHttpRequest();
		HttpResponse& httpResponse = reqState.getHttpResponse();
		reqState.response = constructResponse(httpResponse, httpReq.serverConfig);
		reqState.setStatus(CAN_SEND_MESSAGE);
	}

	ssize_t sendedSsize= sendMessage(fd, reqState.response);
	
	if (sendedSsize <= 0) {
		if (sendedSsize == -1) {
			removeClient(fd);
			return;
		} else {
			std::cerr << "Error sending message to client fd:" << fd << " errno: " << errno << std::endl;
			removeClient(fd);
			return;
		}
	}
	else {
    	reqState.lastActive = std::time(NULL);
		size_t sendedLength = static_cast<size_t>(sendedSsize);
		reqState.response = reqState.response.substr(sendedLength, reqState.response.size() - sendedLength);
		if (reqState.response.size() == 0){
			if (reqState.getHttpResponse().statusCode == 100){
				reqState.setStatus(STILL_GETTING_MSG);
				turnToEventRead(fd);
				return ;
			}
			reqState.setStatus(ALLDONE);
			removeClient(fd);
		}
	}
};