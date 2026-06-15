# include "CGIPipes.hpp"

CGIPipes :: CGIPipes()
{}

CGIPipes :: ~CGIPipes()
{}
bool CGIPipes::setupPipes(int pipeIn[2], int pipeOut[2]) {
    if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1) {
        this->closePipes(pipeIn, pipeOut);
        perror("pipe failed");
        return false;
    }
    fcntl(pipeOut[0], F_SETFL, O_NONBLOCK);
    fcntl(pipeOut[1], F_SETFL, O_NONBLOCK);
    fcntl(pipeIn[0],  F_SETFL, O_NONBLOCK);
    fcntl(pipeIn[1],  F_SETFL, O_NONBLOCK);
    return true;
}

std::string CGIPipes::readOutputNonBlocking(int fd) {
    char buffer[4096];
    std::string output;
    ssize_t bytes = 0;

    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
        output.append(buffer, bytes);
    }
    if (bytes == 0) {
        std::cout << YELLOW << "All bytes read" << RESET << std::endl;
    }

    return (output); 
}

void CGIPipes :: killIfTimeout(pid_t pid, int timeoutSeconds)
{
    time_t start = time(NULL);

    while(true)
    {
        
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);
    
        if(result == pid)
            return; 
        else if(result < 0)
            throw std::runtime_error("waitpid() failed");

        
        if(time(NULL) - start >= timeoutSeconds)
        {
            
            kill(pid, SIGTERM);
            usleep(100000);

            if(waitpid(pid, &status, WNOHANG) == 0)
                kill(pid, SIGKILL);

            waitpid(pid, &status, 0);
            throw std::runtime_error("CGI timeout");
        }
        usleep(1000000);
    }
}

void CGIPipes :: closePipes(int pipeIn[2], int pipeOut[2])
{
    
    if(pipeIn[0] != -1)
        close(pipeIn[0]);
    if(pipeIn[1] != -1)
        close(pipeIn[1]);
    if(pipeOut[0] != -1)
        close(pipeOut[0]);
    if(pipeOut[1] != -1)
        close(pipeOut[1]);
}