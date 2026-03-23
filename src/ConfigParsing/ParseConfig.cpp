/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:43:01 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/26 10:43:03 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseConfig.hpp"

ParseConfig::ParseConfig(){};
ParseConfig::~ParseConfig(){};

std::vector<Token> ParseConfig::tokenizeConfigFile(const std::string &filename) {
	// 1. Leer archivo
	std::string content = readFile(filename);
	if(content.empty()) 
	{
		std::cerr << "Error: Could not read file" << std::endl;
		throw std::runtime_error("Could not read file " + filename);
	}
	
	// std::cout << "File content:\n" << content << "\n\n";
	
	// 2. Tokenizar
	// std::cout << "=== TOKENIZER ===" << std::endl;
	Lexer lexer(content);
	std::vector<Token> tokens = lexer.tokenize();
	return (tokens);
};

void ParseConfig::parseServersAndLocation(std::vector<Token> &tokens) {
	ServerParsing parser;
	size_t pos = 0;
	while (tokens[pos].type != TOKEN_EOF) {
		ServerConfig server = parser.parseServer(tokens, pos);
		this->allServers.push_back(server);
	}
}

void ParseConfig::start(const std::string &fileName) {
	std::vector<Token> tokens = tokenizeConfigFile(fileName);
	parseServersAndLocation(tokens);
};
std::vector<ServerConfig> ParseConfig::getAllServerConfig() {
	return (this->allServers);
};