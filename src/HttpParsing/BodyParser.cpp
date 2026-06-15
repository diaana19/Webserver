#include "BodyParser.hpp"

void BodyParser::parseBody(std::string &str, HttpRequest & httpReq){
	if (checkIfItsAFile(httpReq)) {
		parseFile(str, httpReq);
	} else if (checkIfItsChunked(httpReq)){
		parseChunked(str, httpReq);
	}
	else {
		parseText(str, httpReq);
	}
};

bool BodyParser::checkIfItsAFile(HttpRequest &httpReq) {
	std::string contentType = httpReq.headers["Content-Type"];
	if(contentType.find("multipart/form-data") != std::string::npos)
		return (true);
	return (false);
};

size_t BodyParser::hex_to_decimal(const std::string& hex)
{
    return std::strtoul(hex.c_str(), NULL, 16);
};

bool BodyParser::checkIfItsChunked(HttpRequest &httpReq) {
	std::string contentType = httpReq.headers["Transfer-Encoding"];
	if(contentType.find("chunked") != std::string::npos)
		return (true);
	return (false);
};

void BodyParser::parseChunked(std::string &str, HttpRequest &httpReq) {
    (void)str;
    (void)httpReq;
    File fileConfig;
    std::map<std::string, std::string>::iterator it = httpReq.headers.find("Content-Disposition");
    if (it != httpReq.headers.end()) {
        size_t posFilename = it->second.find("filename=\"");
        if (posFilename != std::string::npos) {
            std::string tmp = "filename=\"";
            posFilename += tmp.size();
            size_t posEndFilename = it->second.find("\"", posFilename);
            fileConfig.filename = it->second.substr(posFilename, posEndFilename - posFilename);
        }
    }
    else {
        time_t now = time(0);
        std::ostringstream oss;
        oss << now;
        fileConfig.filename = oss.str() + ".bin";
       
    }
    fileConfig.file_buffer = str;
    httpReq.allFiles.push_back(fileConfig);
};

void BodyParser::parseFile(std::string &str, HttpRequest &httpReq) {
    std::string boundary;
    std::map<std::string, std::string>::iterator it = httpReq.headers.find("Content-Type");
    if (it != httpReq.headers.end()) {
		
		size_t pos = it->second.find("boundary");
		if (pos != std::string::npos)
        	boundary = it->second.substr((pos + 9), it->second.size());
    }

    size_t pos = 0;
    File fileConfig;
	
    while (pos < str.size()) {
        size_t lineEnd = str.find("\r\n", pos);
        std::string line;
        if (lineEnd == std::string::npos) {
            line = str.substr(pos);
            pos = str.size();
        } else {
            line = str.substr(pos, lineEnd - pos);
            pos = lineEnd + 2;
        }
        
        size_t namePos = line.find("name=\"");
        if (namePos != std::string::npos) {
            namePos += 6;
            size_t nameEnd = line.find('"', namePos);
            if (nameEnd != std::string::npos)
                fileConfig.name = line.substr(namePos, nameEnd - namePos);
        }

        size_t filenamePos = line.find("filename=\"");
        if (filenamePos != std::string::npos) {
            filenamePos += 10;
            size_t filenameEnd = line.find('"', filenamePos);
            if (filenameEnd != std::string::npos) {
                fileConfig.filename = line.substr(filenamePos, filenameEnd - filenamePos);
                std::cout << B_GREEN << "Filename:'" << fileConfig.filename << "'" << RESET << std::endl;
            }
        }

        if (line.find("Content-Type:") != std::string::npos) {
            size_t start = line.find(':');
            if (start != std::string::npos) {
                start += 2; 
                fileConfig.contentType = line.substr(start);
            }
        }

        if (line.empty()) {
            std::string buffer;

            if (!boundary.empty()) {
                std::string endMarker = "--" + boundary;
                size_t bodyEnd = str.find(endMarker, pos);

                if (bodyEnd != std::string::npos) {
                    size_t rawEnd = bodyEnd;
                    if (rawEnd >= 2 && str.substr(rawEnd - 2, 2) == "\r\n")
                        rawEnd -= 2;
                    buffer = str.substr(pos, rawEnd - pos);
                    pos = bodyEnd + endMarker.size();
                    if (pos + 2 <= str.size()) {
                        if (str.substr(pos, 2) == "--")
                            pos += 2;
                        else if (str.substr(pos, 2) == "\r\n")
                            pos += 2;
                    }
                } else {
                    buffer = str.substr(pos);
                    pos = str.size();
                }
            } else {
                buffer = str.substr(pos);
                pos = str.size();
            }
			if (buffer.empty())
            {
                std::cout << B_GREEN << "[UPLOAD]  " << RESET << httpReq.allFiles.size() << " file(s) received" << std::endl;
                return ;
            }
            fileConfig.file_buffer = buffer;
            httpReq.allFiles.push_back(fileConfig);
            fileConfig = File();
        }
    }
    std::cout << B_GREEN << "[UPLOAD]  " << RESET << httpReq.allFiles.size() << " file(s) received" << std::endl;
}

void BodyParser::parseText(std::string &str, HttpRequest &httpReq) {
	httpReq.body = str;
};