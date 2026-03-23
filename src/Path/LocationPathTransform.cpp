/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationPathTransform.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:43:55 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/26 10:43:56 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationPathTransform.hpp"

void LocationPathTransform::changeLocationRoot(std::string &serverRoot, LocationConfig& location) {
	if (location.rootLocation.empty()) {
		location.rootLocation = serverRoot;
		// std::string tmp = serverRoot;
		// if (tmp.size() > 0 && tmp[tmp.size() - 1] != '/')
		// 	tmp += "/";
		// if ()
		// location.rootLocation = serverRoot + location.uri;
	}
};

void LocationPathTransform::iterateInAllLocationCong(ServerConfig &serverConf, std::vector<LocationConfig>&allLocationConf) {
	for (size_t i = 0; i < allLocationConf.size(); i++) {
		changeLocationRoot(serverConf.root, allLocationConf[i]);
	}
};

void LocationPathTransform::transform(std::vector<ServerConfig> &allServerConf){
	for (size_t i = 0; i < allServerConf.size(); i++) {
		iterateInAllLocationCong(allServerConf[i], allServerConf[i].locations);
	}
};

bool LocationPathTransform::checkDupLoc_Check(std::vector<LocationConfig>& allLocConf) {
	for (std::vector<LocationConfig>::iterator it = allLocConf.begin(); it != allLocConf.end(); it++) {
		for (std::vector<LocationConfig>::iterator itCmp = (it + 1); itCmp != allLocConf.end(); itCmp++ ) {
			if (it != itCmp && it->uri == itCmp->uri) {
				std::cout << "   Error: duplicate found for:" << it->uri << std::endl;
				return (true);
			}
		}
	}
	return (false);
}

bool LocationPathTransform::checkDuppLoc_Iterate(std::vector<ServerConfig> &allServConf) {
	for (std::vector<ServerConfig>::iterator it = allServConf.begin(); it != allServConf.end(); it++) {
		if (checkDupLoc_Check(it->locations)) {
			return (true);
		}
	}
	return (false);
};