#include <iostream>
#include <WinSock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

SOCKET Connections[10];
int connectionsCounter = 0;

void ClientHandler(int index) {
	int str_size;

	while (true) {
		recv(Connections[index], (char*)&str_size, sizeof(int), NULL);
		char* msg = new char[str_size + 1];
		msg[str_size] = '\0';
		recv(Connections[index], msg, str_size, NULL);
		for (int i = 0; i < connectionsCounter; i++) {
			if (i == index) continue;
			send(Connections[i], (char*)&str_size, sizeof(int), NULL);
			send(Connections[i], msg, str_size, NULL);
		} 
		delete[] msg;
	}
	
}


int main(int argc, char* argv[]) {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	int size = sizeof(addr);
	SOCKET newCOnnection;
	for(int i = 0 ; i < 10; i++) {
		newCOnnection = accept(sListen, (SOCKADDR*)&addr, &size);
		if (newCOnnection == 0) {
			std::cout << "Connection Failed" << std::endl;
		}
		else {
			std::cout << "Client Connected" << std::endl;
			std::string str = "Hello, world!";
			int size = str.size();


			send(newCOnnection, (char*)&size, sizeof(int), NULL);
			send(newCOnnection, str.c_str(), size, NULL);

			Connections[i] = newCOnnection;
			connectionsCounter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}



}
