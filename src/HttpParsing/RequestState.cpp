/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestState.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 10:11:28 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/12 19:49:11 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestState.hpp"

// RequestState::RequestState(): contentLength(0), hasFinishHeader(false), hasExpect100(false) {

// }

RequestState::RequestState() : status(STILL_GETTING_MSG), readlengthChunked(0), hasFinishReadHeader(false), lastActive(std::time(NULL))
{
    // this->status = STILL_GETTING_MSG;
    // this->lastActive = std::time(NULL);
}

RequestState::~RequestState()
{
}

RequestState::RequestState(const RequestState& other)
    : status(other.status),
      buffer(other.buffer),
      bufferChunked(other.bufferChunked),
      header(other.header),
      body(other.body),
      contentLength(other.contentLength),
      readlengthChunked(other.readlengthChunked),
      hasFinishReadHeader(other.hasFinishReadHeader),
      httpRequest(other.httpRequest),
      httpResponse(other.httpResponse),
      lastActive(other.lastActive)
{
    // this->status = STILL_GETTING_MSG;
    // this->lastActive = std::time(NULL);
};

RequestState& RequestState::operator=(const RequestState& other){
    if (this != &other)
    {
        status = other.status;
        buffer = other.buffer;
        bufferChunked = other.bufferChunked;
        header = other.header;
        body = other.body;
        contentLength = other.contentLength;
        readlengthChunked = other.readlengthChunked;
        hasFinishReadHeader = other.hasFinishReadHeader;
        httpRequest = other.httpRequest;
        httpResponse = other.httpResponse;
        lastActive = other.lastActive;
    }
    // this->status = STILL_GETTING_MSG;
    // this->lastActive = std::time(NULL);
    return (*this);
}

std::string &RequestState::getBuffer()
{
    return (this->buffer);
};

std::string RequestState::getBuffer() const
{
    return (this->buffer);
};


std::string &RequestState::getBufferChunked()
{
    return (this->bufferChunked);
};

std::string RequestState::getBufferChunked() const
{
    return (this->bufferChunked);
};


void RequestState::setBufferChunked(std::string key)
{
    this->bufferChunked = key;
};

std::string &RequestState::getHeader()
{
    return (this->header);
};

std::string RequestState::getHeader() const
{
    return (this->header);
};

std::string &RequestState::getBody()
{
    return (this->body);
};
std::string RequestState::getBody() const
{
    return (this->body);
};

void RequestState::setStatus(int status){
    this->status = status;
};

int RequestState::getStatus() const{
    return (this->status);
};


bool RequestState::checkIfFinishReadingBody(std::string content_LengthReq)
{
    size_t nbContent_LengthReq;
    nbContent_LengthReq = stringToSizeT(content_LengthReq);
    if (nbContent_LengthReq >= this->contentLength)
    {
        return (true);
    }
    return (false);
};

void RequestState::setHeader(std::string header)
{
    this->header = header;
};

void RequestState::setBody(std::string body)
{
    this->body = body;
};

size_t RequestState::getContentLength() const
{
    return (this->contentLength);
};

void RequestState::setContentLength(size_t key)
{
    this->contentLength = key;
};

size_t RequestState::getReadLengthChunked() const
{
    return (this->readlengthChunked);
};

void RequestState::setReadLengthChunked(size_t key)
{
    this->readlengthChunked = key;
};

bool RequestState::getHasFinishReadHeader() const
{
    return (this->hasFinishReadHeader);
};

void RequestState::setHasFinishReadHeader(bool key)
{
    this->hasFinishReadHeader = key;
};

// bool RequestState::getHasConstructMessage() const{
//     return (this->hasConstructMessage);
// };

// void RequestState::setHasConstructMessage(bool key){
//     this->hasConstructMessage = key;
// };


void RequestState::setRequestHttp(const HttpRequest &httpRequest)
{
    this->httpRequest = httpRequest;
};

HttpRequest &RequestState::getHttpRequest()
{
    return (this->httpRequest);
};

HttpRequest RequestState::getHttpRequest() const
{
    return (this->httpRequest);
};

void RequestState::setHttpResponse(const HttpResponse &httpResponse)
{
    this->httpResponse = httpResponse;
};

HttpResponse &RequestState::getHttpResponse()
{
    return (this->httpResponse);
};

HttpResponse RequestState::getHttpResponse() const
{
    return (this->httpResponse);
};
// bool RequestState::getHasFinishReadBody() const{
//     return (this->hasFinishRead);
// };

// void RequestState::setHasFinishReadBody(bool key) {
//     this->hasFinishReadBody = key;
// };

// bool RequestState::getHasExpect100() const{
//     return (this->hasExpect100);
// };

// void RequestState::setHasExpect100(bool key) {
//     this->hasExpect100 = key;
// };
