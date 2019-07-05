#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void)
{
	/* Host and port number of the echo server */
	int sockfd;
	int clientfd;
	int echo_port = 7777;
	char *echo_host = "10.23.48.109";
	struct sockaddr_in *server =
		(struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	struct sockaddr_in *client =
		(struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));

	int bytes;
	char *buf = (char *)malloc(1000);
	int client_size = sizeof(*client);

	/* Set own address */
	server->sin_family = AF_INET;
	server->sin_port = htons(echo_port);	// Note network byte order
	server->sin_addr.s_addr = inet_addr(echo_host);

	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Bind to an address */
	if (bind(sockfd, (struct sockaddr *)server, sizeof(*server)))
		printf("Bind Failed\n");

	/* Enable server mode of socket */
	listen(sockfd, SOMAXCONN);

	/* ...and wait for incoming data */
	printf("Wait for connection to port %u\n", echo_port);

	/* Accept a connection request */
	clientfd = accept(sockfd, (struct sockaddr *)client, &client_size);
	printf("Connected to %s:%u\n\n", inet_ntoa(client->sin_addr),
	       ntohs(client->sin_port));
	printf("Numeric: %u\n", ntohl(client->sin_addr.s_addr));

	while (1) {	/* Endless loop */
		/* Receive transmitted data */
		bytes = read(clientfd, (void *)buf, 1000);
		if (bytes <= 0) {
			close(clientfd);
			printf("Connection closed.\n");
			exit(0);
		}

		printf("Bytes received: %u\n", bytes);
		printf("Text: '%s'\n", buf);

		/* Send Response */
		write(clientfd, buf, bytes);
	}
}
