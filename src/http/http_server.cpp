#include "http/http_server.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "filesystem.hpp"

#define PORT 8135
#define BUFFER_SIZE 104857600

static std::string HTTP_HEADER = "HTTP/1.1 200 OK\r\n\n";

std::string http_server_listen_for_access_code()
{
	int server_fd;
	struct sockaddr_in server_addr;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return "Failed to make socket.";
	}

	// config socket
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	server_addr.sin_port = htons(PORT);

	// bind socket to port
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		return "Failed to bind socket.";
	}

	// listen for connections
	if (listen(server_fd, 10) < 0)
	{
		return "Failed to listen for connection.";
	}

	std::string request;

	while (1) {
		// client info
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_fd = 0;

		// accept client connection
		if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0)
		{
			continue;
		}

		std::cout << "Accepted client.\n";

		char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

		// receive request data from client and store into buffer
		ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

		request = std::string(buffer, bytes_received);

		std::string response = HTTP_HEADER + filesystem_read_whole_file("./assets/access_code_received.html");

		send(client_fd, response.c_str(), response.length(), 0);

		close(client_fd);

		break;
	}

	// Close the socket.
	close(server_fd);

	// Parse the reply to get the access code.
	size_t equalssign_pos = request.find('=');
	size_t space_after_code_pos = request.find(' ', equalssign_pos);

	return request.substr(equalssign_pos + 1, space_after_code_pos - 1 - equalssign_pos);
}
