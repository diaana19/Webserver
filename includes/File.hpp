/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:57:50 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/02 12:57:52 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include <map>
#include <vector>
#include <string>

struct File {
	std::string file_buffer; 
	std::string name; // file
	std::string filename; // something.jpeg
	std::string contentType; // image/jpeg
};