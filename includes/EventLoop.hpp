#pragma once
#ifndef EVENTLOOP_HPP
# define  EVENTLOOP_HPP

#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <vector>
#include <utility>
#include <stdio.h>

#include "Server.hpp"
#include "ClientManager.hpp" 
#include "HttpRequest.hpp"
#include "RequestLine.hpp"
#include "BodyParser.hpp" 
#include "HeadersParsing.hpp"  
#include "HttpResponse.hpp" 
#include "StatusBuilder.hpp" 
#include "HeadersBuilder.hpp" 
#include "BodyBuilder.hpp" 
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

#include <signal.h>
# define TIMEOUT_CLIENT 10 
# define TIMEOUT_CGI_SEC 10 
# define MAXCLIENT 20
# define MAXEVENT_EPOLL 20

extern volatile sig_atomic_t g_shutdown;

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
		std::pair<std::string, int> getHostAndPort(std::string &host);
		int searchIndexServer(std::string ip, int port);
		int searchIndexLocation(ServerConfig &serverConfig, std::string &path);
		LocationConfig searchLocationConfig(HttpRequest &httpRequest);
		void parseChunked(std::string &wholeBody, RequestState &reqState);
		void startUpSignal();
		void startUpCreateEpollfd();
		int addServerFdToEpoll();
		Server* findServer_byFdClient(int fd);
		ServerConfig findTheServerConfig(HttpRequest &httpRequest);
		void addNewClient(int fd);
		void removeClient(int fd);
		void turnToEventRead(int fd);
		void turnToEventWrite(int fd);
		bool AddFdToEpoll(pid_t pid, int pipefd);
		void deleteEventFdFromEpoll(int fd);
		bool shouldCloseConnection(const HttpRequest& req);
		void endingProcess(int fd);
		void turnToEventReadHUP(int fd);
		void checkTimedOutClient();
		bool fdIsCgiOutput(int cgiOutputFd);
		void readCgiOutPut(int fd, uint32_t eventFlags);
		void checkCgiReadyOrTimeOut();
		HttpRequest parseHeader(int fd, RequestState &requeststate);
		void handleRead(int fd);
		void getMessageInFd(int fd);
		void parseWholeReq(int fd, RequestState &currRequestState);
		void receiveChunkedData(int fd, RequestState &currRequestState);
		std::string readChunked(std::string &body, RequestState &reqState, size_t &lastRead);
		void processRequest(int fd);
		HttpResponse getResponseFromMethod(HttpRequest &request, LocationConfig &location, const ServerConfig& server);
		std::string constructResponse(HttpResponse &httpResponse, const ServerConfig &serverConfig);
		void handleWrite(int fd); 
		ssize_t sendMessage(int fd, std::string &response);
		size_t hex_to_decimal(const std::string& hex);
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