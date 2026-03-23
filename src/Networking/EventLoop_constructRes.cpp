/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop_write.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 13:16:13 by dirituay          #+#    #+#             */
/*   Updated: 2026/03/19 12:47:21 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventLoop.hpp"

void EventLoop::processRequest(int fd) {
    RequestState& currRequestState = activeRequests[fd];
    HttpRequest HttpRequest = currRequestState.getHttpRequest();
    std::map<std::string, std::string>::iterator hostIt = HttpRequest.headers.find("Host");
    if (hostIt == HttpRequest.headers.end()) {
        // Sin Host header → 400
        HttpResponse httpresponse;
        httpresponse.statusCode = 400;
		currRequestState.setHttpResponse(httpresponse);
		currRequestState.setStatus(CAN_BUILD_RESPONSE);
        turnToEventWrite(fd);
        return;
    }
	std::pair<std::string, int> ipAndPort = getHostAndPort(hostIt->second);
	int serverIndex = searchIndexServer(ipAndPort.first, ipAndPort.second);
	if (serverIndex == -1) {
		// Server no encontrado → 404
		HttpResponse httpresponse;
		httpresponse.statusCode = 404;
		currRequestState.setHttpResponse(httpresponse);
		currRequestState.setStatus(CAN_BUILD_RESPONSE);
		turnToEventWrite(fd);
		return;
	}
	
    ServerConfig& serverConfig = this->allServersConfig[serverIndex];
	currRequestState.getHttpRequest().serverConfig = serverConfig;
    int locationIndex = searchIndexLocation(serverConfig, HttpRequest.path);
    
    if (locationIndex == -1) {
        // Location no encontrada → 404
        HttpResponse httpresponse;
        httpresponse.statusCode = 404;
		currRequestState.setHttpResponse(httpresponse);
		currRequestState.setStatus(CAN_BUILD_RESPONSE);
        turnToEventWrite(fd);
        return;
    }
    LocationConfig& locationConfig = serverConfig.locations[locationIndex];
    HttpResponse httpresponse = getResponseFromMethod(HttpRequest, 
                                                       locationConfig, 
                                                       serverConfig);
    if (locationConfig.cgi_On) {
		try
		{
			// epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
			turnToEventReadHUP(fd);
			if(httpresponse.statusCode == 500) 
				throw std::runtime_error("Error with the execution of cgi");
			httpresponse.cgiProcess.clientFd = fd;
			// this->runningCgis.insert(std::make_pair(httpresponse.cgiProcess.pid, httpresponse.cgiProcess));
			this->runningCgis[httpresponse.cgiProcess.pid] = httpresponse.cgiProcess; 
			if (AddFdToEpoll(httpresponse.cgiProcess.pid, httpresponse.cgiProcess.outputPipeFd) == false) {
				throw std::runtime_error("Error add outputPipe to epoll");
			}
			currRequestState.setStatus(WAITING_CGI);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			currRequestState.setHttpResponse(httpresponse);
			currRequestState.setStatus(CAN_BUILD_RESPONSE);
			turnToEventWrite(fd);
			return;
		}
		return;
	}
	std::string sessionId;
	std::map<std::string, std::string>::iterator cookieIt = HttpRequest.cookies.find("session_id");
	if (cookieIt != HttpRequest.cookies.end() && _sessionManager.validateSession(cookieIt->second))
	{
		sessionId = cookieIt->second;
		std::cout << "[Session] Existing session: " << sessionId << std::endl;
		CookieSender sender;
		httpresponse.setCookieHeaders.push_back(sender.buildSetCookie("session_id", sessionId, 3600, "/"));
	}
	currRequestState.setHttpResponse(httpresponse);
	currRequestState.setStatus(CAN_BUILD_RESPONSE);
    turnToEventWrite(fd);
};