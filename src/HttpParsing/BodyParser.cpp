/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:58:11 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/10 18:51:54 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BodyParser.hpp"

// bool checkIfFileInBody(httpReq & httpReq) {
// 	if(HttpRequest.ma)
// };

// void parseBody(std::String &str, HttpRequest & httpReq) {
// };

void BodyParser::parseBody(std::string &str, HttpRequest & httpReq){
	if (checkIfItsAFile(httpReq)) {
		parseFile(str, httpReq);
	} else if (checkIfItsChunked(httpReq)){
		parseChunked(str, httpReq);
	}
	else {
		parseText(str, httpReq);
	}
	// JSON DECODER ?? 
	// UTF-8  DECODER ?? 
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
            //std::cout << "Filename with ContentDisp" << fileConfig.filename << std::endl;
        }
    }
    else {
        time_t now = time(0);
        std::ostringstream oss;
        oss << now;
        fileConfig.filename = oss.str() + ".bin";
       // std::cout << "Filename With time:" << fileConfig.filename << std::endl;
    }
    fileConfig.file_buffer = str;
    httpReq.allFiles.push_back(fileConfig);
};

// void BodyParser::parseFile(std::string &str, HttpRequest &httpReq) {
// 	std::istringstream streamBody(str);
// 	std::string boundary;
// 	std::map<std::string, std::string>::iterator it = httpReq.headers.find("boundary");
//     if (it != httpReq.headers.end()) {
//         boundary = it->second;
//     }
// 	std::string line;
// 	File fileConfig;
// 	while(getline(streamBody, line)) {
// 		if (line.find("Content-Disposition:") != std::string::npos) {
// 			line = line.substr(line.find(':') + 2);
// 			// std::cout << "Content-Disposition" << std::endl;
// 			std::istringstream streamLine(line);
// 			std::string word;
// 			std::ostringstream bodyStrm;
// 			while(getline(streamLine, word, ' ')) {
// 				// std::cout << "word[" << word << "]　　" << std::endl;
// 				if (word.find("name") == 0) {
// 					// std::cout << "NAME FOUND:" << std::endl;
// 					int start = word.find('"') + 1;
// 					std::string name = word.substr(start, word.size() - start - 2);
// 					fileConfig.filename = name;
// 					if(httpReq.allFiles)
// 					// std::cout << name << std::endl;
// 				}
// 				if (word.find("filename") == 0) {
// 					// std::cout << "FILENAME FOUND" << std::endl;
// 					int start = word.find('"') + 1;
// 					std::string filename = word.substr(start, word.size() - start - 2);
// 					fileConfig.filename =filename;
// 					// std::cout << filename << std::endl;
// 				}
// 			}
// 		}
// 		std::string buffer;
// 		if (line.empty() || line == "\r"){  // empty line before the file 
// 			// std::cout << "EMPTY FOUND:" << std::endl;
// 			while (true) {
// 				getline(streamBody, line);
// 				if (line == "\r")
// 					break ;
// 				else if (!(boundary.empty()) && line.find(boundary) != std::string::npos) {
// 					// std::cout << "getMultForm_separator FOUND:" << std::endl;
// 					buffer = buffer.substr(0, buffer.size() - 1); // this remove the \n or \r of the file !!
// 					break ;
// 				}
// 				else
// 					buffer += line + '\n';  // we can directly read the file since its a getline so it's stop by line
// 			}
// 			fileConfig.file_buffer += buffer;
// 			httpReq.allFiles.push_back(fileConfig);
			
// 			std::ofstream ofs((fileConfig.filename + "out").c_str(), std::ios::binary);
// 			if (!ofs) {
// 			    std::cerr << "Failed to open output file: " << fileConfig.filename << std::endl;
// 			} else {
// 			    ofs.write(fileConfig.file_buffer.c_str(), fileConfig.file_buffer.size());
// 			    ofs.close();
// 			}
// 			fileConfig = File();
// 		}
// 		// bodyStrm << line << "\n";
// 	}
// };

// void BodyParser::parseFile(std::string &str, HttpRequest &httpReq) {
// 	std::istringstream streamBody(str);
// 	std::string line;
// 	std::string boundary;
// 	File fileConfig;

// 	std::map<std::string, std::string>::iterator it = httpReq.headers.find("boundary");
// 	if (it != httpReq.headers.end()) {
// 		boundary = it->second;
//     }

// 	while(getline(streamBody, line)) {
// 		std::cout << " line[" << line << "]" << std::endl;
// 		size_t namePos = line.find("name=\"");
//     	if (namePos != std::string::npos) {
// 			namePos += 6;
// 			size_t nameEnd = line.find('"', namePos);
// 			fileConfig.name = line.substr(namePos, nameEnd - namePos);
// 		}
//    		 size_t filenamePos = line.find("filename=\"");
//    		 if (filenamePos != std::string::npos) {
// 			filenamePos += 10;
// 			size_t filenameEnd = line.find('"', filenamePos);
// 			fileConfig.filename = line.substr(filenamePos, filenameEnd - filenamePos);
// 			std::cout << "  Filename extracted: '" << fileConfig.filename << "'" << std::endl;
// 		}
// 		if (line.find("Content-Type:") != std::string::npos) {
// 			// std::cout << "CONTENT TYPE FOUND:" << std::endl;
// 			int start = line.find(':') + 2;
// 			std::string contentTypestr = line.substr(start, line.size() - start - 1);
// 			fileConfig.contentType = contentTypestr;
// 			// std::cout << "contentType:" << contentTypestr << std::endl;
// 		}
// 		std::string buffer;
// 		if (line.empty() || getline(streamBody, line)){  // empty line before the file 
// 			// std::cout << "EMPTY FOUND:" << std::endl;
// 			while (getline(streamBody, line)) {
// 				if (line == "\r")
// 					break ;
// 				// else if (!(boundary.empty()) && line.find(boundary) != std::string::npos) {
// 				// 	// std::cout << "getMultForm_separator FOUND:" << std::endl;
// 				// 	buffer = buffer.substr(0, buffer.size()); // this remove the \n or \r of the buffer !!
// 				// 	break ;
// 				// }
// 				else if (line.find("--------") != std::string::npos) {
// 					buffer = buffer.substr(0, buffer.size() - 2); // this remove the \n or \r of the buffer !!
// 					break ;
// 				}
// 				else
// 					buffer += line + '\n';  // we can directly read the file since its a getline so it's stop by line
// 			}
// 			fileConfig.file_buffer = buffer;
// 			httpReq.allFiles.push_back(fileConfig);
// 			fileConfig = File();
// 			buffer.clear();
// 			// test if file is corretly parsed
// 			std::cout <<  "\033[1m\033[31m"  << buffer <<  "\033[0m" << std::endl;
// 			std::ofstream ofs((fileConfig.filename + "out").c_str(), std::ios::binary);
// 			if (!ofs) {
// 			    std::cout << "Failed to open output file: " << fileConfig.filename << std::endl;
// 			} else {
// 			    std::cout << "Succesfully to open output file: " << fileConfig.filename << std::endl;
// 			    ofs.write(fileConfig.file_buffer.data(), fileConfig.file_buffer.size());
// 			    ofs.close();
// 			}
// 		}
// 		// bodyStrm << line << "\n";
// 	}
// };

void BodyParser::parseFile(std::string &str, HttpRequest &httpReq) {
    std::string boundary;
    std::map<std::string, std::string>::iterator it = httpReq.headers.find("Content-Type");
    if (it != httpReq.headers.end()) {
		// std::cout << "the line " << it->second << std::endl;
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

        //std::cout << " line[" << line << "]" << std::endl;

        // --- parse Content-Disposition header ---
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
                start += 2; // skip the ": "
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

            // std::string outName = fileConfig.filename + "out";
            // std::ofstream ofs(outName.c_str(), std::ios::binary);
            // if (!ofs) {
            //     std::cout << "Failed to open output file: " << outName << std::endl;
            // } else {
            //     std::cout << "Successfully opened output file: " << outName << std::endl;
            //     ofs.write(fileConfig.file_buffer.data(),
            //               static_cast<std::streamsize>(fileConfig.file_buffer.size()));
            //     ofs.close();
            // }

            fileConfig = File();
        }
    }
    std::cout << B_GREEN << "[UPLOAD]  " << RESET << httpReq.allFiles.size() << " file(s) received" << std::endl;
}

void BodyParser::parseText(std::string &str, HttpRequest &httpReq) {
	httpReq.body = str;
};