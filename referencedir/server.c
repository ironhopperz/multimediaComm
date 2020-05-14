#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define OPSZ 4
#define BUF_SIZE 1024

void error_handling(char* message);
int Calc(int loop, int* OParray, char op);
char *concatenate(char *a, char *b, char *c);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET serv_sock, clnt_sock;
	SOCKADDR_IN serv_addr, clnt_addr;

	char welcom[] = "test의 서버(test) 입니다 환영합니다";
	char message[30];
	int clnt_addr_len;
	int i, j;
	int count, result;
	int recv_len, recv_clnt;
	
	if(argc != 2)
	{
		printf("Usage %s <port> \n", argv[0]);
		exit(1);
	}
	

	if(WSAStartup( MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(serv_sock == INVALID_SOCKET)
		error_handling("socket() error!");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = PF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (SOCKADDR*)& serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		error_handling("bind() error!");

	if(listen(serv_sock, 5) == INVALID_SOCKET)
		error_handling("listen() error!");

	clnt_addr_len = sizeof(clnt_addr);
	
	clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &clnt_addr_len);
		
	if(clnt_sock == INVALID_SOCKET)
		error_handling("accept() error!");

	for(i=0; i<5; i++)
	{
		recv_clnt = 0;
		count = 0;
		printf("connected............................\n");
		send(clnt_sock, welcom, sizeof(welcom), 0); // 웰컴 환영 메시지지 송신 to client 
		
		recv(clnt_sock, &count, 1, 0);
		
		recv_len = 0;
		while( (OPSZ*count+1) > recv_len)
		{
			recv_clnt = recv(clnt_sock, &message[recv_len], BUF_SIZE-1, 0);
			recv_len += recv_clnt;
		}
		result = Calc(count, (int*)message, message[recv_len-1]);
		send(clnt_sock, (char*)&result, OPSZ, 0);
	}

	closesocket(clnt_sock);
	closesocket(serv_sock);

	WSACleanup();
	return 0;

}


void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int Calc(int loop, int* OParray, char op)
{
	int k;
	int sum = OParray[0];

	switch(op)
	{

	case '+' : 
		for(k=1; k<loop; k++) sum += OParray[k];
		break;

	case '-' : 
		for(k=1; k<loop; k++) sum -= OParray[k];
		break;

	case '*' : 
		for(k=1; k<loop; k++) sum *= OParray[k];
		break;

	default :
		printf("wrong \n");
		break;
	}
	return sum;
}
char *concatenate(char *a, char *b, char *c)
{
  int size = strlen(a) + strlen(b) + strlen(c) + 1;
  char *str = malloc(size);
  strcpy (str, a);
  strcat (str, b);
  strcat (str, c); 

  return str;
}

