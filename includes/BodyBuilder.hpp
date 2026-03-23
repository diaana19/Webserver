/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyBuilder.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:58:26 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/04 19:58:55 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# ifndef BODYBUILDER_HPP
# define BODYBUILDER_HPP

# include <iostream>
# include <string>
# include <fcntl.h>
# include <unistd.h>
# include <sstream>
# include <fstream>
# include <map>

class BodyBuilder
{
	private:
        std::map<int, std::string> errorString;
		void constructMap();
		std::string convertNumberIntoString(int nb);

	public:
		BodyBuilder();
		~BodyBuilder();

		std::string readFile(std::string &pathfile);
		std::string getMimeType(const std::string &pathfile);
		// std::string buildByErrorCode(int errorCode);
};


#endif