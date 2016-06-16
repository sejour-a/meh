//
// Created by Alexandre Sejournant on 16/06/2016.
//

#include <iostream>

#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>

#include <../include/colors.h>

int filefd;
int sockfd;

void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        std::cerr << YELLOW << "SIG_INT caught, closing sockets and terminating program." << COLOR_RESET << std::endl;
        if (filefd)
            close(filefd);
        close(sockfd);
        exit(0);
    }
}

int main(int ac, char **av)
{
    if ((ac < 3) || (ac > 4))
    {
        std::cerr << GREEN << "Usage: ./meh ADDRESS PORT (FILE)" << COLOR_RESET << std::endl;
        return 1;
    }

    signal(SIGINT, sig_handler);
    
    std::string address(av[1]);
    std::cout << GREEN << "Address: " << YELLOW << address << COLOR_RESET << std::endl;
    int port = atoi(av[2]);
    std::cout << GREEN << "Port: " << YELLOW << port << COLOR_RESET << std::endl;
    std::string filename;
    if (ac == 4)
    {
        filename = av[3];
        std::cout << GREEN << "File: " << YELLOW << filename << COLOR_RESET << std::endl;
    }

    if  ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << RED << "Socket error" << COLOR_RESET << std::endl;
        return 2;
    }

    struct hostent *server;

    if ((server = gethostbyname(address.c_str())) == NULL)
    {
        std::cerr << RED << "Can't resolve host " << YELLOW << address << COLOR_RESET << std::endl;
        return 3;
    }

    struct sockaddr_in serv_addr;

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << RED << "Can't connect to " << YELLOW << address << " : " << port << COLOR_RESET << std::endl;
        return 4;
    }

    if (ac == 4)
    {
        if ((filefd = open(filename.c_str(), O_RDONLY)) == -1)
        {
            std::cerr << RED << "Can't open " << YELLOW << filename << COLOR_RESET << std::endl;
            return 5;
        }
    }
    else
        filefd = 0;

    char buff[4096];

    unsigned int r;
    while ((r = read(filefd, buff, 4096)) != 0)
        write(sockfd, buff, r);

    if (filefd)
        close(filefd);
    close(sockfd);

    return 0;
}
