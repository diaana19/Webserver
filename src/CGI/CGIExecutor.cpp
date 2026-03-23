/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dirituay <dirituay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:42:45 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/23 12:26:18 by dirituay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIExecutor.hpp"


CGIExecutor :: CGIExecutor()
{}

CGIExecutor :: ~CGIExecutor()
{}

bool CGIExecutor:: validateScript(const std::string& scriptPath, const std::string& interpreter)
{
    if(access(scriptPath.c_str(), F_OK) != 0)
    {
        std::cerr << RED << "[CGIExecutor] script not found : " << RESET << scriptPath << std::endl;
        return false;
    }
    if(access(scriptPath.c_str(), X_OK) != 0)
    {
        std::cerr << RED << "[CGIExecutor] script not executable. " << RESET << std::endl ;
        return false; 
    }
    if(access(interpreter.c_str(), X_OK) != 0)
    {
        std::cerr << RED << "[CGIExecutor] Intrepreter not found : " << RESET << interpreter << std::endl;
        return false;
    }
    return true;
}

char** CGIExecutor :: buildArgv(const std::string &interpreter, const std::string &scriptPath)
{
    char** argv = new char*[3];
    argv[0] = strdup(interpreter.c_str());
    argv[1] = strdup(scriptPath.c_str());
    argv[2] = NULL;
    return argv;
}

void CGIExecutor :: freeArgv(char** argv)
{
    if(!argv)
        return;
    for(int i = 0; argv[i] != NULL; i++)
        free(argv[i]);
    delete[] argv;
}


pid_t CGIExecutor :: execute( const HttpRequest& request, std::string scriptPath, const std::string &interpreter, char **envp, int pipeIn[2], int pipeOut[2])
{
    (void)request;
    if(!validateScript(scriptPath, interpreter))
        return(-1);

    pid_t pid = fork();

    if(pid < 0)
    {
        std::cerr << RED << "[GCIExecutor] Fork Failed" << RESET << strerror(errno) << std::endl;
        return -1;
    }
    if(pid == 0) //CHILD
    {
        std::cout << B_CYAN << "[CGIExecutor] Im the child process" << RESET << std::endl;
        close(pipeIn[1]);
        close(pipeOut[0]);

        if(dup2(pipeIn[0], STDIN_FILENO) == -1) {
            std::cerr << RED << "[CGIExecutor] dup2(stdin) failed " << RESET << std::endl;
            exit(1);
        }

        close(pipeIn[0]);

        if(dup2(pipeOut[1], STDOUT_FILENO) == -1) {
            std::cerr << RED << "[CGIExecutor] dup2(stdout) failed " << RESET << std::endl;
            exit(1);
        }

        if(dup2(pipeOut[1], STDERR_FILENO) == -1) {
            std::cerr << RED << "[CGIExecutor] dup2(stderr) failed " << RESET << std::endl;
            exit(1);
        }
        close(pipeOut[1]);
        fcntl(STDOUT_FILENO, F_SETFL, 0);
        //In case close all 
        for(int fd =3; fd < 1024; fd++)
            close(fd);

        size_t pos = scriptPath.find_last_of('/');
        if (pos != std::string::npos) {
            std::string dir = scriptPath.substr(0, pos);
            if (chdir(dir.c_str()) != 0) {
                std::cerr << RED << "[CGIExecutor] chdir failed: " << RESET << strerror(errno) << std::endl;
                exit(1);
            }
          //  scriptPath = scriptPath.substr(dir.size() + 1, scriptPath.size() - dir.size());
          scriptPath = scriptPath.substr(pos+1);
        }
        char** argv = buildArgv(interpreter, scriptPath);
        
        execve(interpreter.c_str(), argv, envp);
    
        std::cerr << RED << " [CGIExecutor] exceve failed " << RESET << strerror(errno) << std::endl;
        freeArgv(argv);
        exit(1);
    }
    else
    {
        std::cout << B_CYAN << "[CGIExecutor] Parent process, child PID: " << RESET << pid << std::endl;
       
        close(pipeIn[0]);
        close(pipeOut[1]);

        //fcntl(pipeIn[1], F_SETFL, O_NONBLOCK);
        //if (!request.body.empty()) {
            //ssize_t written = write(pipeIn[1], request.body.c_str(), request.body.size());
            //if(written < 0) {
                //std::cerr << "[CGIExecutor] Write to pipe failed " << strerror(errno) << std::endl;
            //}
        //}

        //close(pipeIn[0]);
        //close(pipeIn[1]);
        //close(pipeOut[1]);
        std::cout << B_CYAN << "[CGIExecutor] Im the parent process: " << RESET << pid << std::endl;
        return pid;
    }
    std::cout << B_CYAN << "CGIExecutor : Interpreter : " << RESET << interpreter << std::endl;
    return -1;
}

