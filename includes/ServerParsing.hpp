#ifndef SERVERPARSING_HPP
#define SERVERPARSING_HPP

#include "Config.hpp"
#include "Token.hpp"
#include "LocationParsing.hpp"
#include <vector>
#include <string>
#include <stdexcept>

class ServerParsing
{
	private:
			std::string intToString(int n);
			int parsePort(const std::vector<Token>& tokens, size_t& pos);
			std::string parseHost( const std::vector<Token>& tokens, size_t& pos);
			std::string parseRoot(const std::vector<Token>& tokens, size_t& pos);
			std::string parseServerName(const std::vector<Token>& tokens, size_t& pos);
			size_t parseClientMaxBodySize(const std::vector<Token>& tokens, size_t& pos);
			std::string parseIndex(const std::vector<Token> &tokens, size_t &pos);
			void parseErrorPage(const std::vector<Token>& tokens, size_t& pos, ServerConfig& config);
			void expectToken(const std::vector<Token>& tokens, size_t& pos, TokenType expected);
			void skipSemicolon(const std::vector<Token>& tokens, size_t& pos);
	public:
		ServerParsing();
		~ServerParsing();
		ServerConfig parseServer(const std::vector<Token>& tokens, size_t& pos);
};

#endif
