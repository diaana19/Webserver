#include "EventLoop.hpp"

EventLoop::EventLoop():epoll_fd(-1)
{
}

EventLoop::~EventLoop()
{
	if (this->epoll_fd != -1)
		close(this->epoll_fd); 
}

int EventLoop::addServerFdToEpoll()
{
	for (std::vector<Server>::iterator it = allServers.begin(); it != allServers.end(); ++it)
	{
		struct epoll_event event;
		event.data.fd = it->getFd();
		event.events = EPOLLIN;
		if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, event.data.fd , &event) == -1)
		{
			perror("epoll_ctl");
			return -1;
		}
	}
	return 0;
}

void EventLoop::addNewClient(int fd) {
	int client_fd = this->_clientManager->acceptNewClient(fd);
	if (client_fd != -1)
	{
		struct epoll_event event;
		event.data.fd = client_fd;
		event.events = EPOLLIN;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
			std::cout << "Failed epoll ctl: " << client_fd << std::endl;
	}
};

bool EventLoop::fdIsCgiOutput(int cgiOutputFd) {
	std::map<pid_t, CGIProcess>::iterator it = this->runningCgis.begin();
	while (it != this->runningCgis.end()) {
		if (it->second.outputPipeFd == cgiOutputFd)
			return (true);
		it++;
	}
	return (false);
};

void  signal_handler(int signal){
	(void)signal;
	g_shutdown = 1;
};

void EventLoop::startUpSignal() {
	signal(SIGINT, signal_handler); 
	signal(SIGTERM, signal_handler);
	signal(SIGPIPE, SIG_IGN); 
};

void EventLoop::startUpCreateEpollfd() {
	this->epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		
		perror("epol_ctl");
	}
};

bool isItTimeOut(time_t cgiTime, long int limitTime) {
	time_t timeNow = time(NULL);
	if ((timeNow - cgiTime) > limitTime) {
		return (true);
	}
	return (false);
};

void EventLoop::checkCgiReadyOrTimeOut() {
	std::map<pid_t, CGIProcess>::iterator it = this->runningCgis.begin();
	CGIPipes cgipipes;
	std::vector<CGIProcess> timed_out;

	while (it != this->runningCgis.end()) {
		CGIProcess & cgiProc = it->second;
		int clientFd = cgiProc.clientFd;
		RequestState &currReqState  = this->activeRequests[clientFd];
		if (it->second.readyToRead && !cgiProc.buffer.empty()) {
			
			PostHandler postHandler;
			HttpResponse httpResponse = postHandler.parseOutput(cgiProc.buffer);
			currReqState.setHttpResponse(httpResponse);
			
			timed_out.push_back(it->second);
		}
		else if (isItTimeOut(it->second.startTime, TIMEOUT_CGI_SEC)) {
			std::cout << "CGI timeout (pid=" << cgiProc.pid << ")" << std::endl;
			kill(it->first, SIGKILL);
			waitpid(it->first, NULL, 0);
			
			HttpResponse httpResponse;
			httpResponse.statusCode = 504;
			currReqState.setHttpResponse(httpResponse);
			
			timed_out.push_back(it->second);
		}
		it++;
	}
	for (size_t i = 0; i < timed_out.size(); i++) {
		deleteEventFdFromEpoll(timed_out[i].outputPipeFd);
		this->runningCgis.erase(timed_out[i].pid);
		int clientFd = timed_out[i].clientFd;
		RequestState &currReqState  = this->activeRequests[clientFd];
		
		currReqState.setStatus(CAN_BUILD_RESPONSE);
		turnToEventWrite(clientFd);
	}
};

void EventLoop::readCgiOutPut(int fd, uint32_t eventFlags) {
	CGIPipes cgipipe;
	CGIProcess* cgi_proc = NULL;
	
	for (std::map<pid_t, CGIProcess>::iterator it = this->runningCgis.begin();
		it != runningCgis.end(); ++it) {
		if (it->second.outputPipeFd == fd) {
			cgi_proc = &it->second;
			break;
		}
	}
	if (!cgi_proc) return;

	cgi_proc->buffer += cgipipe.readOutputNonBlocking(fd);
	if (eventFlags & EPOLLHUP) {
		std::cout << YELLOW << "Time: " << RESET << cgi_proc->startTime << std::endl;
		
		cgi_proc->readyToRead = true;
	}
};

void EventLoop::checkTimedOutClient() {
    time_t timenow = std::time(NULL);
    std::vector<int> toRemove;
    for (std::map<int, RequestState>::iterator it = activeRequests.begin();
         it != activeRequests.end(); ++it) {
        if (timenow - it->second.lastActive > TIMEOUT_CLIENT) {
            it->second.lastActive = timenow;
            toRemove.push_back(it->first);
        }
    }
    for (size_t i = 0; i < toRemove.size(); ++i) {
        int fd = toRemove[i];
        std::map<int, RequestState>::iterator it = activeRequests.find(fd);
        if (it == activeRequests.end())
            continue;

        RequestState &reqState = it->second;
        std::cout << "Timed out Client " << fd << std::endl;
        HttpResponse httpresponse;
        httpresponse.statusCode = 408;
        reqState.setHttpResponse(httpresponse);
        reqState.setStatus(CAN_BUILD_RESPONSE);
        turnToEventWrite(fd);
    }
}

void EventLoop::run()
{
	startUpSignal();
	startUpCreateEpollfd();
	addServerFdToEpoll();
	
	struct epoll_event events[MAXEVENT_EPOLL];
	while (g_shutdown == 0)
	{
		int n = epoll_wait(epoll_fd, events, MAXCLIENT, 1000);
		for (int i = 0; i < n; i++) 
		{
			int fd = events[i].data.fd;
			Server *matchListenServer = findServer_byFdClient(fd);

			if (matchListenServer){ 
				addNewClient(fd);
			}
			else if (fdIsCgiOutput(fd)) {
				
				readCgiOutPut(fd, events[i].events);
			}
			else if (events[i].events & EPOLLIN){ 
				
				handleRead(fd); 
			} 
			if (events[i].events & EPOLLOUT){ 
				
    			handleWrite(fd);
				
			}
			else if (events[i].events & (EPOLLHUP | EPOLLERR)) {
				
				removeClient(fd);
			}
		}
		checkTimedOutClient();
		checkCgiReadyOrTimeOut();
	}
};

bool EventLoop::AddFdToEpoll(pid_t pid, int fd) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;  
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        perror("Error AddFdToEpoll epoll_ctl pipe");
        close(fd);
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
        return (false);
    }
	
    return (true);
};

void EventLoop::deleteEventFdFromEpoll(int fd) {
	if (fd < 0)
		return ;
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
};

void EventLoop::turnToEventReadHUP(int fd) {
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLHUP;
	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
};

void EventLoop::turnToEventRead(int fd) {
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
};

void EventLoop::turnToEventWrite(int fd) {
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLOUT;
	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
};

HttpResponse EventLoop::getResponseFromMethod(HttpRequest &request, LocationConfig &location, const ServerConfig& server) {
	HttpResponse response;
	
	RedirectHandler redirectHandler;
	if (redirectHandler.hasRedirect(location)) {
		response = redirectHandler.handle(location);
		return (response);
	}
	
	if (request.method == "GET") {
		GetHandler getHandler;
		response = getHandler.handle(request, location, server);
	}
	else if (request.method == "POST") {
		PostHandler postHandler;
		response = postHandler.handle(request, location, server);
	}
	else if (request.method == "DELETE") {
		DeleteHandler deleteHandler;
		response = deleteHandler.handle(request, location, server);
	}
	return (response);
};

void EventLoop::endingProcess(int fd) {
	RequestState currRequestState = activeRequests[fd];
    HttpRequest httpRequest = currRequestState.getHttpRequest();
	if (shouldCloseConnection(activeRequests[fd].getHttpRequest())) {
		removeClient(fd);
	}
	else {
		this->activeRequests.erase(fd);
		turnToEventRead(fd);
	}
};

void EventLoop::removeClient(int fd) {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	
	this->activeRequests.erase(fd);
	
	this->_clientManager->removeClient(fd);
};

void EventLoop::addServer(Server &server)
{
	allServers.push_back(server);
};

void EventLoop::setServersConfig(std::vector<ServerConfig> &allServersConfig) {
	this->allServersConfig = allServersConfig;
};

std::vector<ServerConfig>  EventLoop::getServersConfig() const {
	return (this->allServersConfig);
};

void EventLoop::addClientManager(ClientManager *clientManagers)
{
	this->_clientManager = clientManagers;
};

std::vector<Server>& EventLoop::getAllServers()
{
	return (this->allServers);
};