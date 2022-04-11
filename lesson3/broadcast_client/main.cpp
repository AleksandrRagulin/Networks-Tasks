#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>
#include <socket_wrapper/socket_class.h>


int main(int argc, const char * const argv[])
{
    using namespace std::chrono_literals;

    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << "host" <<" <port>" << std::endl;
        return EXIT_FAILURE;
    }

    socket_wrapper::SocketWrapper sock_wrap;

    const int port { std::stoi(argv[2]) };
    const char* host = argv[1];

    std::cout << "Running sending on the port " << port << "...\n";
    
	int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == -1)
    {
        return EXIT_FAILURE;
    }
	std::cout << "1";
	struct sockaddr_in serv_addr = {.sin_family = PF_INET, .sin_port = htons(port)};
		
    inet_pton(AF_INET, host, &serv_addr.sin_addr);

	if (connect(sockfd, reinterpret_cast<const sockaddr *>(&serv_addr),
		sizeof(serv_addr)) < 0)
	{
		return EXIT_FAILURE;
	}
	
	std::string message = "";
    while (true)
    {
    	if (!std::getline(std::cin, message)) continue;
        std::cout << "Sending...:";
        std::cout << "message" << std::endl;
        sendto(sockfd, message.c_str(), message.length(), 0, reinterpret_cast<const sockaddr*>(&serv_addr), sizeof(sockaddr_in));
        std::cout << "Message was sent..." << std::endl;
        std::this_thread::sleep_for(1s);
    }
    close(sockfd);
	
}
