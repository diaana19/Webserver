#pragma once
#include "HttpResponse.hpp"
#include <string>
#include <map>

class StatusBuilder
{
    private:
        std::map<int, std::string> _statusMsg;
        void statusMsgInit();
        std::string getStatusMsg(int code);
    public:
        StatusBuilder();
        ~StatusBuilder();
    
        std::string build(int code);
        std::string build(int code, const std::string &httpVersion);
};
