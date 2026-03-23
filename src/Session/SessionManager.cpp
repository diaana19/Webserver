/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 13:38:09 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/19 13:38:11 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SessionManager.hpp"

std::string SessionManager::createSession(){
	unsigned char buffer[16];
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, buffer, 16);
	close(fd);

	std::stringstream ss;
	for (int i = 0; i < 16; i++) {
		ss << std::hex <<std::setw(2) << std::setfill('0') << (int)buffer[i];
	}
	std::string id = ss.str();
	SessionData newUser;
	sessions[id] = newUser;
	return (id);
};           // Génère un ID unique
bool SessionManager::validateSession(const std::string& sessionId) {
	std::map<std::string, SessionData>::iterator it = this->sessions.find(sessionId);
	if (it == this->sessions.end())
		return (false);
	if (time(NULL) - it->second.lastAccess > SESSION_TIMEOUT) {
		this->sessions.erase(it);
		return (false);
	}
	it->second.lastAccess = time(NULL);
	return (true);
};

SessionData& SessionManager::getSession(const std::string& sessionId) {
	if(validateSession(sessionId)) {
		return (this->sessions.find(sessionId)->second);
	}
	throw std::runtime_error("Invalid session ID");
};

void SessionManager::destroySession(const std::string& sessionId) {
	if(validateSession(sessionId)) {
		size_t nb = this->sessions.erase(sessionId);
		if (nb > 0)
			std::cout << "Session data [" << sessionId <<"] has been erased" << std::endl;
		else
			std::cout << "Error Failed erase Session data [" << sessionId << "]" << std::endl;
		return ;
	}
	std::cout << "Session data [" << sessionId <<"] do not exist" << std::endl;
};
