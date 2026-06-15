#include "ServerParsing.hpp"
#include <stdexcept>
#include <cstdlib>
#include <sstream>

ServerParsing::ServerParsing()
{
	
}

ServerParsing::~ServerParsing()
{

}

std::string ServerParsing::intToString(int n)
{
	std::stringstream ss;
	ss << n;
	return (ss.str()); 
}

void ServerParsing::skipSemicolon(const std::vector<Token>& tokens, size_t& pos)
{
	if (pos >= tokens.size()) 
		throw std::runtime_error("Error: unexpected end of file");
	if (tokens[pos].type != TOKEN_SEMICOLON)
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": it was expected ';'"); 
	pos++;
}


void ServerParsing::expectToken(const std::vector<Token> &tokens, size_t &pos, TokenType expected)
{
	if (pos >= tokens.size())
		throw std::runtime_error("Error: unexpected end of file");
	if (tokens[pos].type != expected)
		throw std::runtime_error("Error line: " + intToString(tokens[pos].line) + ": unexpected token");
}


int ServerParsing::parsePort(const std::vector<Token>& tokens, size_t& pos)
{
	if (tokens[pos].type == TOKEN_LISTEN)
		pos++;
	expectToken(tokens, pos, TOKEN_NUMBER);
	int line = tokens[pos].line; 
	std::string portStr = tokens[pos].value; 
	int port = atoi(portStr.c_str());
	pos++;
	skipSemicolon(tokens, pos);
	if (port < 1 || port > 65535) 
		throw std::runtime_error("Error line: " + intToString(line) + ": port out of range(1-65535)");
	return (port);
}


std::string ServerParsing::parseHost( const std::vector<Token>& tokens, size_t& pos)
{
	if(tokens[pos].type == TOKEN_HOST)
		pos++;
	if (tokens[pos].type != TOKEN_STRING && tokens[pos].type != TOKEN_NUMBER)
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": expected host value");
	std::string host = tokens[pos].value;
	pos++;
	skipSemicolon(tokens, pos);
	return (host);
}

#include <sys/stat.h>

static bool isDirectory(const std::string &path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;

    return S_ISDIR(info.st_mode);
}


std::string ServerParsing::parseRoot(const std::vector<Token> &tokens, size_t &pos)
{
	if (tokens[pos].type == TOKEN_ROOT)
		pos++;

	if (tokens[pos].type != TOKEN_PATH && tokens[pos].type != TOKEN_STRING)
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": expected root path");

	std::string root = tokens[pos].value;
	
	int line = tokens[pos].line;

	pos++;
	skipSemicolon(tokens, pos);
	
	
	if(!isDirectory(root))
		throw std::runtime_error("Error line " + intToString(line) + ": invalid root directory " + root);
	
	return (root);
}



std::string ServerParsing::parseIndex(const std::vector<Token> &tokens, size_t &pos)
{
	if (tokens[pos].type == TOKEN_INDEX)
		pos++;
	if (tokens[pos].type != TOKEN_PATH && tokens[pos].type != TOKEN_STRING)
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": expected index file");
	std::string index = tokens[pos].value;
	pos++;
	skipSemicolon(tokens, pos);
	return (index);
}


std::string ServerParsing::parseServerName(const std::vector<Token>& tokens, size_t& pos)
{
	if (tokens[pos].type == TOKEN_SERVER_NAME)
		pos++;
	expectToken(tokens, pos, TOKEN_STRING);
	std::string serverName = tokens[pos].value;
	pos++;
	skipSemicolon(tokens, pos);
	return (serverName);
}


size_t ServerParsing::parseClientMaxBodySize(const std::vector<Token> &tokens, size_t &pos)
{
	if (tokens[pos].type == TOKEN_CLIENT_MAX_BODY_SIZE)
		pos++;
	expectToken(tokens, pos, TOKEN_NUMBER);
	int line = tokens[pos].line;
	std::string value = tokens[pos].value; 
	size_t number = 0;
	size_t i = 0;
	while (i < value.size() && isdigit(value[i]))
	{
		number = number * 10 + (value[i] - '0');
		i++; 
	}
	if (i < value.size()) 
	{
		char unit = value[i];
		if (unit == 'K' || unit == 'k')
			number = number * 1024;
		else if (unit == 'M' || unit == 'm')
			number = number * 1024 * 1024;
		else if (unit == 'G' || unit == 'g')
			number = number * 1024 * 1024 * 1024;
		else
			throw std::runtime_error("Error line " + intToString(line) + ": invalid unit (use K, M, or G)");
	}
	pos++;
	skipSemicolon(tokens, pos);
	return (number);
}


void ServerParsing::parseErrorPage(const std::vector<Token> &tokens, size_t &pos, ServerConfig &config)
{
	if (tokens[pos].type == TOKEN_ERROR_PAGE)
		pos++;
	std::vector<int> errorCodes; 
	while (pos < tokens.size() && tokens[pos].type == TOKEN_NUMBER)
	{
		int code = atoi(tokens[pos].value.c_str());
		errorCodes.push_back(code); 
		pos++;
	}
	if (errorCodes.empty()) 
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": expected error code");
	if (tokens[pos].type != TOKEN_PATH && tokens[pos].type != TOKEN_STRING) 
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": expected error page path");
	std::string errorPath = tokens[pos].value; 
	pos++;
	for (size_t i = 0; i < errorCodes.size(); i++) 
		config.errorPages[errorCodes[i]] = errorPath; 
	skipSemicolon(tokens, pos);
}


ServerConfig ServerParsing::parseServer(const std::vector<Token> &tokens, size_t &pos)
{
	expectToken(tokens, pos, TOKEN_SERVER);
		pos++;
	expectToken(tokens, pos, TOKEN_LBRACE);
		pos++;
	ServerConfig config; 
	while (pos < tokens.size() && tokens[pos].type != TOKEN_RBRACE)
	{
		std::string directive = tokens[pos].value;
		if (directive == "listen")
			config.port = parsePort(tokens, pos);
		else if (directive == "host")
			config.host = parseHost(tokens, pos);
		else if (directive == "root")
			config.root = parseRoot(tokens, pos);
		else if (directive == "server_name")
			config.serverName = parseServerName(tokens, pos);
		else if (directive == "client_max_body_size")
			config.clientMaxBodySize = parseClientMaxBodySize(tokens, pos);
		else if (directive == "index")
			config.index = parseIndex(tokens, pos);
		else if (directive == "error_page")
			parseErrorPage(tokens, pos, config);
		else if (directive == "location") {
			LocationParsing locationParser;
			locationParser.resetLocationConfig();
			std::vector<Token>::const_iterator it = tokens.begin() + pos;
			locationParser.parseLocation(it);
			pos = locationParser.actualizePos(tokens);
			LocationConfig newLocation = locationParser.getLocationConfig();
			config.locations.push_back(newLocation);
		}
		else 
			throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": unknown directive '" + directive + "'");
	}
	expectToken(tokens, pos, TOKEN_RBRACE);
	pos++;
	if (config.port == 0)
   	throw std::runtime_error("Error: server block missing 'listen' directive");
	return (config);
}