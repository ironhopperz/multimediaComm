#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define OPSZ 4
#define BUF_SIZE 1024

void error_handling(char* message);


int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET  clnt_sock;
	SOCKADDR_IN  clnt_addr;

	char welcom[BUF_SIZE];
	char message[30];

	int i;
	int count, result;
	
	if(argc != 3)
	{
		printf("Usage %s <ip> <port> \n", argv[0]);
		exit(1);
	}
	

	if(WSAStartup( MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");

	clnt_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(clnt_sock == INVALID_SOCKET)
		error_handling("socket() error!");

	
	memset(&clnt_addr, 0, sizeof(clnt_addr));
	clnt_addr.sin_family = PF_INET;
	clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
	clnt_addr.sin_port = htons(atoi(argv[2]));

	if(connect(clnt_sock, (SOCKADDR*)&clnt_addr, sizeof(clnt_addr)) == SOCKET_ERROR)
		error_handling("connect() error!");

	recv(clnt_sock, welcom, BUF_SIZE, 0);
	printf("connected...............to server \n");
	fputs(welcom, stdout);
	
	printf("\n operation count?:  ");
	fflush(stdin);
	scanf("operand %d : \n", &count);
	message[0] = (char)count;

	for(i=0; i<count; i++)
	{
		printf("%d  : ", i+1);
		fflush(stdin);
		scanf("%d", (int*)&message[1+OPSZ*i]);
	}
	fflush(stdin);
	printf("operator (+or-) : ");
	scanf("%c", &message[count*OPSZ+1]);
	send(clnt_sock, message, 2+OPSZ*count, 0);
	
	recv(clnt_sock, &result, OPSZ, 0);
	printf("result : %d \n", result);
	closesocket(clnt_sock);
	WSACleanup();

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
