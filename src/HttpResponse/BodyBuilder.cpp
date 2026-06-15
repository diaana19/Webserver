#include "BodyBuilder.hpp"

std::string BodyBuilder::convertNumberIntoString(int nb) {
	std::ostringstream oss;
	oss << nb;
	std::string lineNb = oss.str();
	return (lineNb);
}

BodyBuilder::BodyBuilder(){
}

BodyBuilder::~BodyBuilder(){
}

std::string BodyBuilder::readFile(std::string &pathfile) {
	std::ifstream fileIS(pathfile.c_str(), std::ios::binary);
	if (!(fileIS.is_open())) {
		return ("");
	}
	std::ostringstream buffer;
	buffer << fileIS.rdbuf();
	buffer << "\r\n";
	return (buffer.str());
};

std::string BodyBuilder::getMimeType(const std::string &pathfile) {
	static std::map<std::string, std::string> mapExt;
	mapExt[".html"] = "text/html";
	mapExt[".css"]  = "text/css";
	mapExt[".js"]   = "application/javascript";
	mapExt[".png"]  = "image/png";
	mapExt[".jpg"]  = "image/jpeg";
	mapExt[".jpeg"] = "image/jpeg";
	mapExt[".gif"]  = "image/gif";
	mapExt[".txt"]  = "text/plain";
	mapExt[".json"] = "application/json";
	mapExt[".pdf"]  = "application/pdf";
	
	size_t iDot = pathfile.find_last_of(".");
	if (iDot == std::string::npos) {
		return ("application/octet-stream");
	}
	std::string fileExt = pathfile.substr(iDot);

	std::map<std::string,std::string>::const_iterator it = mapExt.find(fileExt);
	if (it != mapExt.end())
		return (it->second);
	return ("application/octet-stream");
};