/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 16:57:16 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/20 02:41:14 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

static void pipe_or_throw(int *pipedes)
{
    if (pipe(pipedes) < 0) {
        perror("cgi pipe");
        throw std::exception();
    }
}

static void dup_or_throw(int fd1, int fd2)
{
    if (dup2(fd1, fd2) < 0) {
        perror("cgi dup2");
        throw std::exception();
    }
}

static void close_or_throw(int fd)
{
    if (close(fd) < 0) {
        perror("cgi close");
        throw std::exception();
    }
}

static void exec_or_throw(char const *path, char const **argv, char const **envp)
{
    if (execve(path, (char *const *)argv, (char *const *)envp) < 0) {
        perror("cgi execve");
        throw std::exception();
    }
}

CGI::CGI(void) {}

CGI::CGI(Request &request, std::string &binary, std::string &resource) :
    _request(request),
    _binary(binary),
    _resource(resource)
{
}

CGI::CGI(CGI const &src) { *this = src; }

CGI::~CGI(void) {}

CGI &CGI::operator=(CGI const &rhs)
{
    if (this != &rhs) {
        _binary   = rhs._binary;
        _resource = rhs._resource;
        _request  = rhs._request;
    }
    return (*this);
}

void CGI::execute(void)
{
    std::vector<std::string> environment = _formatEnvironment();
    for (size_t i = 0; i < environment.size(); i++)
    {
        std::cout << environment[i] << std::endl;
    }
    pipe_or_throw(_pipedes);
    if (_request.getMethod() == "POST")
        write(_pipedes[1], _request.getBody().c_str(), _request.getBody().length());
    _pid = fork();
    if (_pid == 0) {
        _childRoutine();
    } else if (_pid > 0) {
        _parentRoutine();
    } else {
        perror("cgi fork");
        throw std::exception();
    }
}

std::string const &CGI::getOutput(void) const { return _output; }

void CGI::_childRoutine(void)
{
    std::vector<std::string> environment = _formatEnvironment();
    std::string const path   = _resolveBinaryPath();
    char const       *argv[] = {path.c_str(), _resource.c_str(), NULL};
    char const       *envp[] = {
        environment[0].c_str(),
        environment[1].c_str(),
        environment[2].c_str(),
        environment[3].c_str(),
        environment[4].c_str(),
        environment[5].c_str(),
        environment[6].c_str(),
        environment[7].c_str(),
        environment[8].c_str(),
        environment[9].c_str(),
        NULL};

    dup_or_throw(_pipedes[0], STDIN_FILENO);
    close_or_throw(_pipedes[0]);
    dup_or_throw(_pipedes[1], STDOUT_FILENO);
    close_or_throw(_pipedes[1]);
    exec_or_throw(argv[0], argv, envp);
}

std::vector<std::string> CGI::_formatEnvironment(void)
{
    std::vector<std::string> envpVector;
    std::map<std::string, std::string> headers = this->_request.getHeaders();

    std::ifstream resourceContent(this->_resource.c_str());
    std::string   body((std::istreambuf_iterator<char>(resourceContent)), std::istreambuf_iterator<char>());
    std::stringstream ss;
    ss << body.length();

    envpVector.resize(10);
    
    envpVector[0] = "REQUEST_METHOD=" + this->_request.getMethod();
    envpVector[1] = "QUERY_STRING=" + this->_request.getQueryString();
    envpVector[2] = "CONTENT_TYPE=text/html";
    envpVector[3] = "CONTENT_LENGTH=" + ss.str();

    if (headers.count("cookie") > 0) {
        envpVector[4] = "HTTP_COOKIE=" + headers["cookie"];
    }
    if (headers.count("user-agent") > 0) {
        envpVector[5] = "HTTP_USER_AGENT=" + headers["user-agent"];
    }
    envpVector[6] = "DOCUMENT_ROOT=/examples"; // hardcoded, fix later
    envpVector[7] = "SCRIPT_FILENAME=" + this->_resource.substr(1);
    envpVector[8] = "SCRIPT_NAME=" + this->_resource.substr(this->_resource.find_last_of('/') + 1);
    envpVector[9] = "REDIRECT_STATUS=200";

    return envpVector;
}

void CGI::_parentRoutine(void)
{
    int const bufsize = 4096;
    char      buffer[bufsize + 1];
    int       ret = 1;

    close_or_throw(_pipedes[1]);
    waitpid(_pid, NULL, 0);
    while (ret) {
        ret = read(_pipedes[0], buffer, bufsize);
        if (ret < 0) {
            perror("cgi read");
            throw std::exception();
        }
        buffer[ret] = '\0';
        _output += buffer;
    }
    close_or_throw(_pipedes[0]);
}

std::string CGI::_resolveBinaryPath(void)
{
    std::string                              resolve;
    std::vector<std::string>                 splitPath;
    std::vector<std::string>::const_iterator it;

    if (_binary.find_first_of('/') != std::string::npos)
        return (_binary);
    splitPath = ftstring::split(std::getenv("PATH"), ':');
    for (it = splitPath.begin(); it != splitPath.end(); ++it) {
        resolve.assign((*it) + "/" + _binary);
        if (access(resolve.c_str(), X_OK) == 0)
            break;
    }
    return (resolve);
}
