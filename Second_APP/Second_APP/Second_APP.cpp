#include <iostream>
#include <winsock2.h> 
#include <ws2tcpip.h> 
#include <string>

using namespace std;

int main() {
	WSADATA WsaData;
	int err = WSAStartup(0x0101, &WsaData);
	if (err == SOCKET_ERROR) {
		cout << "WSAStartup() failed:" << GetLastError() << endl;
		return 1;
	}

	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;

	SOCKADDR_IN from;
	int fromlen = sizeof(from);

	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr.s_addr);

	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int server_socket1, size, connection1;

	sin.sin_port = htons(20000);
	err = bind(server_socket, (LPSOCKADDR)&sin, sizeof(sin));
	if (err < 0) cout << "Linking error!" << endl;

	sin.sin_port = htons(20000);
	err = listen(server_socket, SOMAXCONN);
	server_socket1 = accept(server_socket, (struct sockaddr*)&from, &fromlen);
	if (server_socket1 < 0) cout << "Error with accepting!" << endl;

	sin.sin_port = htons(25000);
	connection1 = connect(client_socket, (LPSOCKADDR)&sin, sizeof(sin));
	if (connection1 < 0) cout << "Error with connection!" << endl;
	else cout << "Succesfull connection!" << endl;

	int i = 1;
	while (1) {
		cout << "--------Iteration " << i << "--------" << endl;
		char* received_msg = new char[255];
		char* buffer = new char[255];

		cout << "Client2 <- Client1" << endl;
		size = recv(server_socket1, received_msg, 255, 0);

		if (strcmp(received_msg, "Server has stopped his work!;") == 0) {
			delete[] received_msg;
			delete[] buffer;
			break;
		}

		cout << received_msg << " Length of received string = " << size << endl;
		cout << "Client's Port = " << ntohs(from.sin_port) << endl;
		cout << "Client's IP address = " << inet_ntop(AF_INET, &from.sin_addr, buffer, 255) << endl;

		cout << "Client2 -> Client3" << endl;
		send(client_socket, received_msg, size, 0);

		delete[] received_msg;
		delete[] buffer;

		i++;
		cout << "\n";
	}
	cout << "Server has stopped his work!" << endl;
	send(client_socket, "Server has stopped his work!", 28, 0);

	closesocket(server_socket1);
	closesocket(client_socket);
	closesocket(server_socket);

}