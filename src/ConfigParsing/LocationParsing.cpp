#include "LocationParsing.hpp"

std::string convertNumberIntoString(int nb) {
	std::ostringstream oss;
	oss << nb;
	std::string lineNb = oss.str();
	return (lineNb);
}

LocationParsing::LocationParsing(){
	allParsingFunction["location"] = &LocationParsing::parseFirstLineLocation;
	allParsingFunction["root"] = &LocationParsing::parseRootLocation;
	allParsingFunction["index"] = &LocationParsing::parseIndex;
	allParsingFunction["autoindex"] = &LocationParsing::parseAutoindex;
	allParsingFunction["allow_methods"] = &LocationParsing::parseAllow_methods;
	allParsingFunction["upload_path"] = &LocationParsing::parseUploadPath;
	allParsingFunction["return"] = &LocationParsing::parseRedirect_code;
	allParsingFunction["cgi_path"] = &LocationParsing::parseCgi_path;
	allParsingFunction["cgi_interpreter"] = &LocationParsing::parseCgi_interpreter;
	allParsingFunction["cgi_ext"] = &LocationParsing::parseCgi_extentions;
};

LocationParsing::~LocationParsing(){};

LocationConfig LocationParsing::getLocationConfig() const{
	return (this->newLocationConfig); 
};

size_t LocationParsing::actualizePos(const std::vector<Token> &tokens) {
	size_t pos = static_cast<size_t>(this->lastPos - tokens.begin());
	return (pos);
};

void LocationParsing::resetLocationConfig() {
	
};


bool LocationParsing::parseFirstLineLocation(std::vector<Token>::const_iterator& it) {
	if (it->type == TOKEN_PATH) {
		RequestLine normalizer;
		this->newLocationConfig.uri = normalizer.normalizePath(it->value);
	}
	it++;
	if (it->type != TOKEN_LBRACE) {
		return (false);
	}
	it++;
	return (true);
};

void LocationParsing::parseLocation(std::vector<Token>::const_iterator& it) {
	while (it->type != TOKEN_EOF)
	{
		if (it->type == TOKEN_RBRACE)
		{
			it++;
			lastPos = it;
			return ;
		}
		std::map<std::string, bool (LocationParsing::*)(std::vector<Token>::const_iterator&)>::const_iterator itMap = allParsingFunction.find(it->value);
		if (itMap != allParsingFunction.end())
		{
			it++;
			if (((this->*(itMap->second))(it)) == true)
			{
				continue;
			}
			else 
			{
				std::string lineNb = convertNumberIntoString(it->line);
				throw std::runtime_error("Error: Unknow type in location line " + lineNb + " for the token " + it->value + "\n");
			}
		}
		it++;
	}
};

#include <sys/stat.h>

static bool isDirectory(const std::string &path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0) {
        
        return false;
    }

    return (info.st_mode & S_IFDIR) != 0;
}

bool LocationParsing::parseRootLocation(std::vector<Token>::const_iterator& it) {
	if (it->type != TOKEN_UNKNOWN && it->type != TOKEN_PATH && it->type != TOKEN_STRING) {
		return false;
	}
	std::string root = it->value;
	int line = it->line;
	it++;
	if (it->type != TOKEN_SEMICOLON) {
		return (false);
	}
	it++;

	
	
	if(!isDirectory(root)) {
		throw std::runtime_error("Error line" + convertNumberIntoString(line) + ": invalid root directory in location " + root);
	}

	this->newLocationConfig.rootLocation = root;
	return (true);
};

bool LocationParsing::parseIndex(std::vector<Token>::const_iterator& it) {
	size_t numberOfArgument = 0;
	while(it->type != TOKEN_EOF) {
		if(it->type == TOKEN_SEMICOLON)
			break;
		if(it->type == TOKEN_PATH || it->type == TOKEN_STRING) {
			this->newLocationConfig.index.push_back(it->value);
			numberOfArgument++;
		}
		else
			return (false);
		it++;
	}
	if (numberOfArgument == 0 || it->type != TOKEN_SEMICOLON)
		return (false);
	it++;
	return (true);
};

bool LocationParsing::parseAllow_methods(std::vector<Token>::const_iterator& it) {
	size_t numberOfArgument = 0;
	while(it->type != TOKEN_EOF) {
		if(it->type == TOKEN_SEMICOLON)
			break;
		if( it->value == "GET" || it->value == "POST" || it->value == "DELETE") {
			this->newLocationConfig.allow_methods.push_back(it->value);
			numberOfArgument++;
		}
		else 
			return (false);
		it++;
	}
	if (numberOfArgument == 0 || it->type != TOKEN_SEMICOLON)
		return (false);
	it++;
	return (true);
};
bool LocationParsing::parseUploadPath(std::vector<Token>::const_iterator& it) {
	if (it->type == TOKEN_PATH || it->type == TOKEN_UNKNOWN || it->type == TOKEN_STRING)
		this->newLocationConfig.uploadPath = it->value;
	it++;
	if (it->type != TOKEN_SEMICOLON) {
		return (false);
	}
	it++;
	return (true);
};


bool LocationParsing::parseAutoindex(std::vector<Token>::const_iterator& it) {
	if (it->type == TOKEN_STRING) {
		if (it->value == "on")
			this->newLocationConfig.autoindex = true;
		else if (it->value == "off")
			this->newLocationConfig.autoindex = false;
		else
			return (false);
	}
	it++;
	if (it->type != TOKEN_SEMICOLON) {
		return (false);
	}
	it++;
	return (true);
};

bool LocationParsing::parseRedirect_code(std::vector<Token>::const_iterator& it) {
	long long redirectCode;
	if (it->type == TOKEN_NUMBER) {
		redirectCode = std::atoll(it->value.c_str());
		if (redirectCode >= 0 && redirectCode <= 600) 
			this->newLocationConfig.redirect_code = static_cast<int>(redirectCode);
	}
	else if (it->type == TOKEN_PATH) {
		this->newLocationConfig.redirect_target = it->value;
		it++;
		if (it->type != TOKEN_SEMICOLON) {
			return (false);
		}
		return (true);
	}
	else
		return (false);
	it++;
	if (it->type == TOKEN_PATH || it->type == TOKEN_STRING) {
		this->newLocationConfig.redirect_target = it->value;
	}
	it++;
	if (it->type != TOKEN_SEMICOLON) {
		return (false);
	}
	it++;
	return (true);

};

bool LocationParsing::parseCgi_path(std::vector<Token>::const_iterator& it) {
	if(it->type == TOKEN_PATH || it->type == TOKEN_UNKNOWN || it->type == TOKEN_STRING)
		this->newLocationConfig.cgi_path = it->value;
	else
		return (false);
	it++;
	if (it->type != TOKEN_SEMICOLON)
		return (false);
	it++;
	return (true);
};

bool LocationParsing::parseCgi_interpreter(std::vector<Token>::const_iterator& it) {
    if(it->type == TOKEN_PATH || it->type == TOKEN_UNKNOWN || it->type == TOKEN_STRING)
        this->newLocationConfig.cgi_interpreter = it->value;
    else
        return (false);
    
    it++;
    
    if (it->type != TOKEN_SEMICOLON)
        return (false);
    
    this->newLocationConfig.cgi_On = true;
    
    it++;
    return (true);
}

bool LocationParsing::parseCgi_extentions(std::vector<Token>::const_iterator& it) {
	size_t numberOfArgument = 0;
	while(it->type != TOKEN_EOF) {
		if(it->type == TOKEN_SEMICOLON)
			break;
		if(it->value == ".py" || it->value == ".php"|| it->value == ".sh") {
			this->newLocationConfig.cgi_extension.push_back(it->value);
			numberOfArgument++;
		}
		else {
			return (false);
		}
		it++;
	}
	if (numberOfArgument == 0 || it->type != TOKEN_SEMICOLON)
		return (false);
	it++;
	return (true);
};