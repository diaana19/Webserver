/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationPathTransform.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:42:12 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/26 10:42:13 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef LOCATIONPATHTRANSFORM_HPP
# define  LOCATIONPATHTRANSFORM_HPP

# include <iostream>
# include <vector>
# include <stdlib.h>     /* atoll */

# include "Config.hpp"

class LocationPathTransform{
	private:
		void iterateInAllLocationCong(ServerConfig &serverConf, std::vector<LocationConfig>&allLocationConf);
		bool checkDupLoc_Check(std::vector<LocationConfig>& allLocConf);
		
	public :
			
		LocationPathTransform(){};
		~LocationPathTransform(){};

		void changeLocationRoot(std::string &serverRoot, LocationConfig& location);
		void transform(std::vector<ServerConfig> &allServerConf);
		
		bool checkDuppLoc_Iterate(std::vector<ServerConfig> &allServConf);
};

#endif