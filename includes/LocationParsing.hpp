/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParsing.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 17:40:35 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/19 18:54:07 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef LOCATIONPARSING_HPP
# define  LOCATIONPARSING_HPP

# include <iostream>
# include <vector>
# include <stdlib.h>     /* atoll */

# include "Config.hpp"
# include "Token.hpp"
# include "RequestLine.hpp"

class LocationParsing{
	private:
		LocationConfig newLocationConfig;
		std::vector<Token>::const_iterator lastPos;
		std::map<std::string, bool (LocationParsing::*)(std::vector<Token>::const_iterator&)> allParsingFunction;
	
		bool parseFirstLineLocation(std::vector<Token>::const_iterator& it);
		bool parseRootLocation(std::vector<Token>::const_iterator& it);
		bool parseIndex(std::vector<Token>::const_iterator& it);
		bool parseAllow_methods(std::vector<Token>::const_iterator& it);
		bool parseUploadPath(std::vector<Token>::const_iterator& it);
	
		bool parseAutoindex(std::vector<Token>::const_iterator& it);
	
		bool parseRedirect_code(std::vector<Token>::const_iterator& it);
		bool parseCgi_path(std::vector<Token>::const_iterator& it);
		bool parseCgi_interpreter(std::vector<Token>::const_iterator& it);
		bool parseCgi_extentions(std::vector<Token>::const_iterator& it);
		
	public :
			
		LocationParsing();
		~LocationParsing();

		LocationConfig getLocationConfig() const; // return a copy locationConfig when it's done 

		//use this to start parsing token
		void parseLocation(std::vector<Token>::const_iterator& it);
		
		// get the last position of the vector Token
		size_t actualizePos(const std::vector<Token> &tokens);
		void resetLocationConfig();
};

#endif