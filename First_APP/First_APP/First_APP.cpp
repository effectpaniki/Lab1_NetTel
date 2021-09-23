#include <iostream>
#include <winsock2.h> 
#include <ws2tcpip.h> 

using namespace std;

int main() {
	WSADATA WsaData;
	int err = WSAStartup(0x0101, &WsaData);
	if (err == SOCKET_ERROR) {
		cout << "WSAStartup() failed:" << GetLastError() << endl;
		return 1;
	}

	int client_socket1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(20000);

	inet_pton(AF_INET, "192.168.0.100", &sin.sin_addr.s_addr);

	int connection1 = connect(client_socket1, (LPSOCKADDR)&sin, sizeof(sin));
	if (connection1 < 0) cout << "Error with connection!" << endl;
	else cout << "Succesfull connection!" << endl;

	cout << "Client1 -> Client2" << endl;
	char send_msg[] = "Hello from client1!";
	send(client_socket1, send_msg, sizeof(send_msg), 0);
	closesocket(client_socket1);

	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sin.sin_port = htons(15000);
	inet_pton(AF_INET, "192.168.0.100", &sin.sin_addr.s_addr);

	err = bind(server_socket, (LPSOCKADDR)&sin, sizeof(sin));
	if (err < 0) cout << "Linking error!" << endl;
	err = listen(server_socket, SOMAXCONN);

	SOCKADDR_IN from;
	int fromlen = sizeof(from);
	int server_socket1 = accept(server_socket, (struct sockaddr*)&from, &fromlen);
	if (server_socket1 < 0) cout << "Error with accepting!" << endl;

	cout << "Client1 <- Client3" << endl;
	char* received_msg = new char[255];
	int size = recv(server_socket1, received_msg, 255, 0);
	cout << received_msg << " Length of received string = " << size << endl;

	delete[] received_msg;
	closesocket(server_socket1);

	closesocket(server_socket);
}