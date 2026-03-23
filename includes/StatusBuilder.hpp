/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusBuilder.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:42:36 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/26 10:42:37 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "HttpResponse.hpp"
#include <string>
#include <map>

//Just for the line of status
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
