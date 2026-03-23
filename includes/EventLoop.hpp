/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 13:22:44 by dirituay          #+#    #+#             */
/*   Updated: 2026/03/12 21:17:36 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef EVENTLOOP_HPP
# define  EVENTLOOP_HPP

#include <sys/epoll.h>
#include <unistd.h> // close
#include <iostream> // cout/cerr
#include <string>
#include <sys/socket.h> // recv
#include <vector>
#include <utility> // std::pair
#include <stdio.h>

#include "Server.hpp"
#include "ClientManager.hpp" 
#include "HttpRequest.hpp"  // request client
#include "RequestLine.hpp"  // parser The first line of request
#include "BodyParser.hpp" // parser
#include "HeadersParsing.hpp"  // parser
#include "HttpResponse.hpp" 
#include "StatusBuilder.hpp" // head http code status
#include "HeadersBuilder.hpp" // head http response
#include "BodyBuilder.hpp" // body http response not needed here ??
#include "RequestState.hpp"
#include "GetHandler.hpp"
#include "PostHandler.hpp"
#include "RedirectHandler.hpp"
#include "DeleteHandler.hpp"
#include "GetHandler.hpp"
#include "Config.hpp"
#include "ErrorHandler.hpp"
#include "CGIPipes.hpp"
#include "CookieSender.hpp"
#include "SessionManager.hpp"
#include "Colors.hpp"

#include <signal.h> // global variable sig_atomic_t
# define TIMEOUT_CLIENT 10 
# define TIMEOUT_CGI_SEC 10 
# define MAXCLIENT 20
# define MAXEVENT_EPOLL 20

extern volatile sig_atomic_t g_shutdown; // global variable declaration

std::string getFirstLine(std::string &fullRequest);
void printHeaders(const HttpRequest &request);
void printRequest(const HttpRequest &req);

class EventLoop
{
	private:
		int epoll_fd;
		std::vector<Server> allServers;
		std::vector<ServerConfig> allServersConfig;
		ClientManager* _clientManager;
		std::map<int, RequestState> activeRequests;
		std::map<pid_t, CGIProcess> runningCgis;
		
		//parsing
		std::pair<std::string, int> getHostAndPort(std::string &host);
		int searchIndexServer(std::string ip, int port);
		int searchIndexLocation(ServerConfig &serverConfig, std::string &path);
		LocationConfig searchLocationConfig(HttpRequest &httpRequest);
		void parseChunked(std::string &wholeBody, RequestState &reqState);
		
		//Server setup
		void startUpSignal();
		void startUpCreateEpollfd();
		int addServerFdToEpoll();
		
		//Server 
		Server* findServer_byFdClient(int fd);
		ServerConfig findTheServerConfig(HttpRequest &httpRequest);
		void addNewClient(int fd);
		void removeClient(int fd);
		void turnToEventRead(int fd);
		void turnToEventWrite(int fd);
		bool AddFdToEpoll(pid_t pid, int pipefd);
		void deleteEventFdFromEpoll(int fd);
		bool shouldCloseConnection(const HttpRequest& req);
		void endingProcess(int fd); // should be call at the end after sending msg to client
		void turnToEventReadHUP(int fd);
		void checkTimedOutClient();

		//CGI
		bool fdIsCgiOutput(int cgiOutputFd);
		void readCgiOutPut(int fd, uint32_t eventFlags);
		void checkCgiReadyOrTimeOut();
		
		//Data handler // header and body
		HttpRequest parseHeader(int fd, RequestState &requeststate); // Parse and give a new httpRequest
		void handleRead(int fd);
		void getMessageInFd(int fd);
		void parseWholeReq(int fd, RequestState &currRequestState);

		// std::string readChunked(std::string &body, RequestState &reqState);
		void receiveChunkedData(int fd, RequestState &currRequestState);
		std::string readChunked(std::string &body, RequestState &reqState, size_t &lastRead);
		
		//Process request 
		void processRequest(int fd);
		HttpResponse getResponseFromMethod(HttpRequest &request, LocationConfig &location, const ServerConfig& server);
		std::string constructResponse(HttpResponse &httpResponse, const ServerConfig &serverConfig);
		
		//SendingResponse
		void handleWrite(int fd); // Put all class needed to write the response back here
		ssize_t sendMessage(int fd, std::string &response);
		
		//UTILS 
		size_t hex_to_decimal(const std::string& hex);
		//session manager
		SessionManager _sessionManager;
		
	public:
		EventLoop();
		~EventLoop();
		
		void run();
		
		void addServer(Server& server);
		void addClientManager(ClientManager* clientManager);
		void setServersConfig(std::vector<ServerConfig> &allServersConfig);

		std::vector<ServerConfig> getServersConfig() const;
		std::vector<Server>& getAllServers();

};

#endif