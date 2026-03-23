/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 17:39:46 by dianarituay       #+#    #+#             */
/*   Updated: 2026/01/23 17:39:49 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string.h>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>

enum TokenType {
	TOKEN_SERVER,
	TOKEN_LOCATION,
	TOKEN_LISTEN,
	TOKEN_ROOT,
	TOKEN_INDEX,
	TOKEN_AUTOINDEX,
	TOKEN_ERROR_PAGE,
	TOKEN_CLIENT_MAX_BODY_SIZE,
	TOKEN_SERVER_NAME,
	TOKEN_HOST,
	TOKEN_ALLOW_METHODS,
	TOKEN_RETURN,

	TOKEN_STRING,
	TOKEN_NUMBER,
	TOKEN_PATH,

	TOKEN_LBRACE,   // {
	TOKEN_RBRACE,   // }
	TOKEN_SEMICOLON,// ;
	TOKEN_EOF,
	TOKEN_UNKNOWN,

	TOKEN_CGI_PATH,
	TOKEN_CGI_EXT
};

//Auxiliar function x read
std::string readFile(const std::string& filename);

struct Token {
	TokenType type;
	std::string value;
	int line;

	//constructor x incicializacion
	Token() : type(TOKEN_UNKNOWN), value(""), line(0)
	{}
	Token(TokenType t, const std::string& v, int l) : type(t), value(v), line(l) {}
};


class Lexer
{
	private:
		std::string	_content;
		size_t	_pos;
		int	_line;

		char finger();
		char advance();
		void skipWhitespace();
		void skipComment();
		std::string readWord();
		std::string readString();
		std::string readNumber();

	public:
		Lexer();
		Lexer(const std::string& content);
		Token nextToken();
		std::vector <Token> tokenize();
		~Lexer();
};
