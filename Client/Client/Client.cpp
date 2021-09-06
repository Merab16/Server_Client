
#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

SOCKET Connection;

void ClientHandler() {
	int str_size;
	while (true) {
		recv(Connection, (char*)&str_size, sizeof(int), NULL);
		char* msg = new char[str_size + 1];
		msg[str_size] = '\0';
		recv(Connection, msg, str_size, NULL);
		std::cout << msg << std::endl;
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

	Connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server" << std::endl;
		return 1;
	}

	std::cout << "Connetced!\n";

	int size;
	recv(Connection, (char*)&size, sizeof(int), NULL);
	char* msg = new char[size + 1];
	msg[size] = '\0';
	recv(Connection, msg, size, NULL);
	std::cout << msg << std::endl;

	CreateThread(NULL, NULL,(LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	std::string str;
	while (true) {
		
		std::getline(std::cin, str);
		int str_size = str.size();

		send(Connection, (char*)&str_size, sizeof(int), NULL);
		send(Connection, str.c_str(), str_size, NULL);
		Sleep(10);

	}
	delete[]msg;
	return 0;
}