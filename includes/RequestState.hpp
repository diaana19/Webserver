#pragma once
#ifndef REQUESTSTATE_HPP
# define  REQUESTSTATE_HPP

#include <iostream>
#include <string>
#include <ctime>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

size_t stringToSizeT(std::string &str);

enum status{
	STILL_GETTING_MSG = 0,
	HAS_FINISH_PARSEHEADER = 0,
	HAS_FINISH_PARSEBODY = 1,
	WAITING_CGI,
	CAN_BUILD_RESPONSE,
	CAN_SEND_MESSAGE,
	ALLDONE,
};

class RequestState {
    private:
        int status;
        std::string buffer;
        std::string bufferChunked;
        std::string header;
        std::string body;
        size_t contentLength;
        size_t readlengthChunked;
        bool hasFinishReadHeader;
        // bool hasConstructMessage;
        HttpRequest httpRequest;
        HttpResponse httpResponse;
        // bool hasExpect100;
        
   public:
        std::string response;
        time_t lastActive;
        RequestState();
        ~RequestState();
        RequestState(const RequestState&copy);
        RequestState &operator=(const RequestState &copy);
        std::string& getBuffer();
        std::string getBuffer() const;
        std::string& getBufferChunked();
        std::string getBufferChunked() const;
        void setBufferChunked(std::string key);
        std::string& getHeader();
        std::string getHeader() const;
        std::string& getBody();
        std::string getBody() const;
        void setStatus(int status);
        int getStatus() const;
        void setHeader(std::string header);
        void setBody(std::string body);
        size_t getContentLength() const;
        void setContentLength(size_t key);
        size_t getReadLengthChunked() const;
        void setReadLengthChunked(size_t key);
        bool checkIfFinishReadingBody(std::string contentLengthReq);
        bool getHasFinishReadHeader() const;
        void setHasFinishReadHeader(bool key);
        void setRequestHttp(const HttpRequest& httpRequest);
        HttpRequest& getHttpRequest() ;
        HttpRequest getHttpRequest() const;
        void setHttpResponse(const HttpResponse& httpResponse);
        HttpResponse& getHttpResponse() ;
        HttpResponse getHttpResponse() const;
};

#endif