/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 17:40:14 by dianarituay       #+#    #+#             */
/*   Updated: 2026/01/23 17:40:16 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Token.hpp"

Lexer :: Lexer() : _content(""), _pos(0), _line(0)
{}

Lexer :: Lexer(const std::string& content) : _content(content) , _pos(0), _line(1)
{}
Lexer :: ~Lexer()
{}
char Lexer :: finger()
{
	if(_pos >= _content.length())
		return '\0';
	return _content	[_pos]; //where is my finger
}

char Lexer :: advance()
{
	if (_pos >= _content.length())
		return '\0';
	char c = _content[_pos++];
	if(c == '\n')
		_line++;
	return c;
}

void Lexer :: skipWhitespace()
{
	while(finger() != '\0' && std::isspace(finger()))
		advance();
}
/*Saltar comentarios , empiezan con #*/
void Lexer :: skipComment()
{
	if(finger()  == '#' )
	{
		while (finger() != '\0' && finger() != '\n')
			advance();
	}
}

std::string Lexer :: readWord()
{
	std::string word;

	while(finger() != ' ' && finger() != ';' && finger() != '{' && finger() != '\0' && finger() != '\n' && finger() != '\t')
		word += advance();
	return word;
}

std::string Lexer :: readNumber()
{
	std::string num;

	while(std::isdigit(finger()) || finger() == '.')
		num += advance();

	//omitiendo parte de mg kb etc
	if(finger() == 'M' || finger() == 'm' || 
		finger() == 'K' || finger() == 'k' ||
		finger() == 'G' || finger() == 'g')
		num += advance(); 
	return num;
}

std::string Lexer::readString()
{
	std::string str;
	char quote = advance();

	while(finger() != quote && finger() != '\0')
	{
		if(finger() == '\\')
		{
			advance();
			str += advance();
		}
		else
			str += advance();
	}
	if(finger() == quote)
		advance();
	return str;
}

Token Lexer::nextToken()
{
	skipWhitespace();
	skipComment();
	skipWhitespace();

	if(_pos >= _content.length())
		return Token(TOKEN_EOF, "", _line);

	char c = finger();

	//symbols psecials
	if( c=='{') {
		advance();
		return Token(TOKEN_LBRACE, "{", _line);
	}
	if( c=='}') {
		advance();
		return Token(TOKEN_RBRACE, "}", _line);
	}
	if( c==';') {
		advance();
		return Token(TOKEN_SEMICOLON, ";", _line);
	}

	//string between quotes(comillas)
	if(c == '"' || c == '\'') {
		std::string str = readString();
		return Token(TOKEN_STRING, str, _line);
	}

	//Numbers
	if(std::isdigit(c)) {
		std::string num = readNumber();
		return Token(TOKEN_NUMBER, num, _line);
	}

	//new path 
	if(c == '.' || c == '/') {
		std::string word = readWord();
		return Token(TOKEN_PATH, word, _line);
	}

	//Keywords and identifiers
	if(std::isalpha(c)) {
		std::string word = readWord();

		if (word == "server")
			return Token(TOKEN_SERVER, word, _line);
		if (word == "listen")
			return Token(TOKEN_LISTEN, word, _line);
		if (word == "server_name")
			return Token(TOKEN_SERVER_NAME, word, _line);
		if(word == "host")
			return Token(TOKEN_HOST, word, _line);
		if (word == "root")
			return Token(TOKEN_ROOT, word, _line);
		if (word == "index")
			return Token(TOKEN_INDEX, word, _line);
		if (word == "error_page")
			return Token(TOKEN_ERROR_PAGE, word, _line);
		if (word == "location")
			return Token(TOKEN_LOCATION, word, _line);
		if (word == "allow_methods")
			return Token(TOKEN_ALLOW_METHODS, word, _line);
		if (word == "autoindex")
			return Token(TOKEN_AUTOINDEX, word, _line);
		if (word == "client_max_body_size")
			return Token(TOKEN_CLIENT_MAX_BODY_SIZE, word, _line);
		if (word == "return")
			return Token(TOKEN_RETURN, word, _line);
		if (word == "cgi_path")
			return Token(TOKEN_CGI_PATH,  word, _line);
		if (word == "cgi_ext")
			return Token(TOKEN_CGI_EXT, word, _line);
		//PATHS start with /
		if(word.find ('/') != std::string::npos || word.find('.') != std::string::npos) 
			return Token(TOKEN_PATH, word, _line);
		
		return Token(TOKEN_STRING, word, _line);
	}

	char unknow = advance();
	return Token(TOKEN_UNKNOWN, std::string(1, unknow), _line);
}

std::vector<Token> Lexer::tokenize()
{
	std::vector<Token> tokens;

	Token token;

	while((token = nextToken()).type != TOKEN_EOF)
	{
		tokens.push_back(token);
	}

	tokens.push_back(Token(TOKEN_EOF, "", _line));
	return tokens;
}
