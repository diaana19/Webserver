/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:41:41 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/17 23:14:41 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <unistd.h> //fork, execve, pipes
#include <sys/types.h> //pid_t
#include <string>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h> // rlimit getrlimit

#include "CGIPipes.hpp"
#include "HttpRequest.hpp"
#include "Colors.hpp"

class CGIExecutor
{
    private:
        bool validateScript(const std::string& scriptPath, const std::string& interpreter);
        char** buildArgv(const std::string &interpreter, const std::string &scriptPath);
        void freeArgv(char** argv);
    public:
        CGIExecutor();
        ~CGIExecutor();

        pid_t execute(const HttpRequest& request, std::string scriptPath, const std::string &interpreter, char **envp, int pipeIn[2], int pipeOut[2]);
        
};