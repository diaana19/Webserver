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
	return _content	[_pos]; 
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