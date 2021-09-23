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

	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(25000);

	inet_pton(AF_INET, "192.168.0.100", &sin.sin_addr.s_addr);

	err = bind(server_socket, (LPSOCKADDR)&sin, sizeof(sin));
	if (err < 0) cout << "Linking error!" << endl;
	err = listen(server_socket, SOMAXCONN);

	SOCKADDR_IN from;
	int fromlen = sizeof(from);
	int server_socket1 = accept(server_socket, (struct sockaddr*)&from, &fromlen);
	if (server_socket1 < 0) cout << "Error with accepting!" << endl;

	cout << "Client3 <- Client2" << endl;
	char* received_msg = new char[255];
	int size = recv(server_socket1, received_msg, 255, 0);
	closesocket(server_socket1);

	cout << received_msg << " Length of received string = " << size << endl;
	cout << "Client's Port = " << ntohs(from.sin_port) << endl;
	char* buffer = new char[255];
	cout << "Client's IP address = " << inet_ntop(AF_INET, &from.sin_addr, buffer, 255) << endl;
	delete[] buffer;

	int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sin.sin_port = htons(15000);

	int connection1 = connect(client_socket, (LPSOCKADDR)&sin, sizeof(sin));
	if (connection1 < 0) cout << "Error with connection!" << endl;
	else cout << "Succesfull connection!" << endl;

	cout << "Client3 -> Client1" << endl;
	send(client_socket, received_msg, size, 0);
	closesocket(client_socket);

	delete[] received_msg;
	closesocket(server_socket);
}

