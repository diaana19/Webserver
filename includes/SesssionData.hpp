#pragma once
# ifndef SESSIONDATA_HPP
# define SESSIONDATA_HPP

# include <ctime>
# include <map>
# include <string>

struct SessionData {
	std::map<std::string, std::string>data;
	time_t created;
	time_t lastAccess;

	SessionData(): created(time(NULL)), lastAccess(time(NULL)){};
	SessionData(const SessionData&copy):data(copy.data), created(copy.created), lastAccess(copy.lastAccess){};
};

#endif