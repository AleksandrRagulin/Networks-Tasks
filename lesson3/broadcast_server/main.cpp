#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>
#include <socket_wrapper/socket_class.h>


const size_t buffer_size = 256;


int main(int argc, char const * const argv[])
{
    using namespace std::chrono_literals;

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        return EXIT_FAILURE;
    }

    socket_wrapper::SocketWrapper sock_wrap;

    const int port { std::stoi(argv[1]) };
    char buffer[buffer_size] = {};
    std::cout << "Receiving messages on the port " << port << "...\n";

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int newsockfd = 0;
    
    
    struct sockaddr_storage client_addr = {0};
	socklen_t client_len = sizeof(client_addr);
    
    struct sockaddr_in serv_addr = {0};
	if (sockfd < 0)
	{
		close(sockfd);
		return EXIT_FAILURE;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(sockfd, reinterpret_cast<const sockaddr*>(&serv_addr),
		sizeof(serv_addr));
	listen(sockfd, 10);
	
	newsockfd = accept(sockfd, reinterpret_cast<sockaddr *>(&client_addr),
			&client_len);
		if (newsockfd < 0)
		{
			close(newsockfd);
			close(sockfd);
			throw std::runtime_error("ERROR on accept");
		}

	while (true)
	{
		
		if (recv(newsockfd, buffer, sizeof(buffer) - 1, 0) < 0)
        {
            std::cerr << sock_wrap.get_last_error_string() << std::endl;
            close(newsockfd);
			close(sockfd);
            return EXIT_FAILURE;
        }
        
        std::cout << buffer << std::endl;
	
	}

}
