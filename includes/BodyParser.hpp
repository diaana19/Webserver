#pragma once
# ifndef BODYPARSER_HPP
# define BODYPARSER_HPP

# include <iostream>
# include <string>
# include "HttpRequest.hpp"
# include "Colors.hpp"
# include <fstream>
# include <sstream>
# include <cstdlib>

class BodyParser
{
	private:	
		bool checkIfItsAFile(HttpRequest &httpReq);
		bool checkIfItsChunked(HttpRequest &httpReq);
		void parseText(std::string &str, HttpRequest &httpReq);
		void parseFile(std::string &str, HttpRequest &httpReq);
		size_t hex_to_decimal(const std::string& hex);
		void parseChunked(std::string &str, HttpRequest &httpReq);

	public:
		BodyParser(){};
		~BodyParser(){};

		void parseBody(std::string &str, HttpRequest & httpReq);
};

#endif