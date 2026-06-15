#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <ctime>

class Client
{
	private:
		int _fd;
		std::string _ip;
		int _port; 
		time_t _lastActivity; 
	public:
		Client(int fd, const std::string& ip, int port);
		//getters
		int getFd() const;
		std::string getIp() const;
		int getPort() const;
		void updateActivity();
};

#endif