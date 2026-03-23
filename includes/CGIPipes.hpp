/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIPipes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vali <vali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 18:51:02 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/06 20:59:35 by vali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CGIPIPE_HPP
# define  CGIPIPE_HPP

# include <iostream>
# include "Config.hpp"
// #include "ConfigParser.hpp"
# include <sys/epoll.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/wait.h>
#include "Colors.hpp"

class CGIPipes {
	public:
    CGIPipes();
    ~CGIPipes();
    bool setupPipes(int pipeIn[2], int pipeOut[2]);
    std::string readOutputNonBlocking(int fd);
    void killIfTimeout(pid_t pid, int timeoutSeconds);
    void closePipes(int pipeIn[2], int pipeOut[2]); // close file descriptor
};

#endif