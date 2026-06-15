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
		std::string convertNumberIntoString(int nb);

	public:
		BodyBuilder();
		~BodyBuilder();

		std::string readFile(std::string &pathfile);
		std::string getMimeType(const std::string &pathfile);
};

#endif