#pragma once
# ifndef REDIRECTHANDLER_HPP
# define REDIRECTHANDLER_HPP

# include <iostream>
# include <string>
# include "Config.hpp"
# include "HttpResponse.hpp"
# include <fstream>
# include <sstream>
# include <cstdlib>

class RedirectHandler
{
	public:
		RedirectHandler();
		~RedirectHandler();
		bool hasRedirect(LocationConfig &location);
		HttpResponse handle(LocationConfig &location);
};

#endif