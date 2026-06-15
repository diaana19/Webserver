#ifndef CGIPIPE_HPP
# define  CGIPIPE_HPP

# include <iostream>
# include "Config.hpp"
# include <sys/epoll.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/wait.h>
#include "Colors.hpp"

class CGIPipes 
{
	public:
    CGIPipes();
    ~CGIPipes();
    bool setupPipes(int pipeIn[2], int pipeOut[2]);
    std::string readOutputNonBlocking(int fd);
    void killIfTimeout(pid_t pid, int timeoutSeconds);
    void closePipes(int pipeIn[2], int pipeOut[2]);
};

#endif