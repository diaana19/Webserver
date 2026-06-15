#pragma once
# ifndef SESSIONMANAGER_HPP
# define SESSIONMANAGER_HPP

# include <iostream>
# include <map>
# include <stdlib.h>
# include <sstream>
# include "SesssionData.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <iomanip>


# define SESSION_TIMEOUT 3600

class SessionManager {
	private:
		std::map<std::string, SessionData> sessions;

	public:
		SessionManager(){};
		~SessionManager(){};
    
    std::string createSession();       
    bool validateSession(const std::string& sessionId);
    SessionData& getSession(const std::string& sessionId);
    void destroySession(const std::string& sessionId);
};

#endif