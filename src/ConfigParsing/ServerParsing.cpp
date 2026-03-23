/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerParsing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:47:14 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/13 16:18:00 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	return (ss.str()); //asi obtenemos el str
}
//verifica q el sgt token es un ; y la pos++ en el sgt token
void ServerParsing::skipSemicolon(const std::vector<Token>& tokens, size_t& pos)
{
	if (pos >= tokens.size()) //si pos es <= al vector, accede a lugar inexiste
		throw std::runtime_error("Error: unexpected end of file");
	if (tokens[pos].type != TOKEN_SEMICOLON)
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": it was expected ';'"); //convierte number line a str, 5 a "5"
	pos++;
}

//verifica cualquier tipo de token
void ServerParsing::expectToken(const std::vector<Token> &tokens, size_t &pos, TokenType expected)
{
	if (pos >= tokens.size())
		throw std::runtime_error("Error: unexpected end of file");
	if (tokens[pos].type != expected)
		throw std::runtime_error("Error line: " + intToString(tokens[pos].line) + ": unexpected token");
}

//parsear listen 8080; c_str(atoi recibe const char *)
int ServerParsing::parsePort(const std::vector<Token>& tokens, size_t& pos)
{
	if (tokens[pos].type == TOKEN_LISTEN)
		pos++;
	expectToken(tokens, pos, TOKEN_NUMBER);
	int line = tokens[pos].line; //guardar la linea para los throw
	std::string portStr = tokens[pos].value; //leer valor del token
	int port = atoi(portStr.c_str());
	pos++;
	skipSemicolon(tokens, pos);
	if (port < 1 || port > 65535) //valida rango
		throw std::runtime_error("Error line: " + intToString(line) + ": port out of range(1-65535)");
	return (port);
}

//TOKEN_HOST/ TOKEN_STRING o TOKEN_NUMBER "127.0.0.1"
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

//docs/fusion_web/;
std::string ServerParsing::parseRoot(const std::vector<Token> &tokens, size_t &pos)
{
	if (tokens[pos].type == TOKEN_ROOT)
		pos++;

	if (tokens[pos].type != TOKEN_PATH && tokens[pos].type != TOKEN_STRING)
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": expected root path");

	std::string root = tokens[pos].value;
	//ADD NEW CORRECTION 
	int line = tokens[pos].line;

	pos++;
	skipSemicolon(tokens, pos);
	
	//ADD FOR validation
	if(!isDirectory(root))
		throw std::runtime_error("Error line " + intToString(line) + ": invalid root directory " + root);
	
	return (root);
}


//index.html;
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

//parsea server_name localhost;
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

//convierte el size de client.. a bytes
size_t ServerParsing::parseClientMaxBodySize(const std::vector<Token> &tokens, size_t &pos)
{
	if (tokens[pos].type == TOKEN_CLIENT_MAX_BODY_SIZE)
		pos++;
	expectToken(tokens, pos, TOKEN_NUMBER);
	int line = tokens[pos].line;
	std::string value = tokens[pos].value; //"10M"
	size_t number = 0;
	size_t i = 0;
	while (i < value.size() && isdigit(value[i]))
	{
		number = number * 10 + (value[i] - '0');
		i++; //str to int
	}
	if (i < value.size()) //ver si hay K, M, G
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

//config. q pqg HTML mostrar cuando ocurre un error HTTP
void ServerParsing::parseErrorPage(const std::vector<Token> &tokens, size_t &pos, ServerConfig &config)
{
	if (tokens[pos].type == TOKEN_ERROR_PAGE)
		pos++;
	std::vector<int> errorCodes; //vector temp para guardar los codigos
	while (pos < tokens.size() && tokens[pos].type == TOKEN_NUMBER)
	{
		int code = atoi(tokens[pos].value.c_str());
		errorCodes.push_back(code); //guarda en el vector
		pos++;
	}
	if (errorCodes.empty()) //si es vacio, error
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": expected error code");
	if (tokens[pos].type != TOKEN_PATH && tokens[pos].type != TOKEN_STRING) //lee el path
		throw std::runtime_error("Error line " + intToString(tokens[pos].line) + ": expected error page path");
	std::string errorPath = tokens[pos].value; //lee el token
	pos++;
	for (size_t i = 0; i < errorCodes.size(); i++) //asigna el path a todos los codigos
		config.errorPages[errorCodes[i]] = errorPath; //[500], [500, 502], ... modifica direc. en config, guarda en el map de errorPages
	skipSemicolon(tokens, pos);
}

//parsea todo server{} y devuelve un serverconfig con la info
ServerConfig ServerParsing::parseServer(const std::vector<Token> &tokens, size_t &pos)
{
	expectToken(tokens, pos, TOKEN_SERVER);
		pos++;
	expectToken(tokens, pos, TOKEN_LBRACE);
		pos++;
	ServerConfig config; //serverconfig vacio
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

/* => 1 kilobyte -> 1024 bytes
1 megabyte -> 1024 * 1024 bytes -> 1048576 bytes
1 gigabyte -> 1024 * 1024 * 1024 bytes -> 1073741824 bytes
*/