#pragma once

#ifndef LOCATIONPATHTRANSFORM_HPP
# define  LOCATIONPATHTRANSFORM_HPP

# include <iostream>
# include <vector>
# include <stdlib.h>  

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