/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:42:17 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/26 10:42:18 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSECONFIG_HPP
# define  PARSECONFIG_HPP

#include "Config.hpp"
// #include "ConfigParser.hpp"
#include "ServerParsing.hpp"

class ParseConfig {
	private :
		std::vector<Token> tokenizeConfigFile(const std::string &filename);
		void parseServersAndLocation(std::vector<Token> &tokens);
		std::vector<ServerConfig> allServers;
		
	public :
		ParseConfig();
		~ParseConfig();

		void start(const std::string &fileName);
		std::vector<ServerConfig> getAllServerConfig();
};

#endif