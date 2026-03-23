/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 13:39:15 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/19 13:39:17 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    
    std::string createSession();           // Génère un ID unique
    bool validateSession(const std::string& sessionId);
    SessionData& getSession(const std::string& sessionId);
    void destroySession(const std::string& sessionId);
};

#endif