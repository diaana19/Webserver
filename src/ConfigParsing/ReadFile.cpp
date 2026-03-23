/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadFile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 17:40:01 by dianarituay       #+#    #+#             */
/*   Updated: 2026/01/23 17:40:03 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Token.hpp"

std::string readFile(const std::string& filename)
{
	if(filename.empty() || filename.length() == 0)
		return ""; //error
	int fd = open(filename.c_str(), O_RDONLY);
	if (fd < 0)
		return "";

	std::string result;
	char buffer[1024];
	ssize_t bytes;

	while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
		result.append(buffer, bytes); //append better than + , just fill the necesary
	if(bytes < 0)
	{
		perror ("bytes");
		return "";
	}

	close(fd);
	return result;
}