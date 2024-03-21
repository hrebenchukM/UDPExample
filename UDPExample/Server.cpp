#include <winsock2.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>



using namespace std;

#define MAX_CLIENTS 30
#define DEFAULT_BUFLEN 4096

#pragma comment(lib, "ws2_32.lib") // Winsock library
#pragma warning(disable:4996) // ��������� �������������� _WINSOCK_DEPRECATED_NO_WARNINGS

SOCKET server_socket;

vector<string> history;


////////////////////////////////////////////////////////////////////
string food[] = {"hamburger", "sprite", "free potato","ice cream"};
int timefood[] = {5, 1, 3,2};
int moneyfood[] = { 50, 35, 40,45 };




struct ClientRequest {
	SOCKET clientSocket;
	string order;
};

ClientRequest requestArray[MAX_CLIENTS]; // ������ �������� �� ��������
int numRequests = 0; // ���������� �������� � �������/                     



                

void Order(SOCKET clientSocket, char* clientMessage)
{
	string order = clientMessage;
	int totalTime = 0;
	int totalMoney = 0;
	int time1, time2, time3, time4;
	int money1, money2, money3, money4;

	if ((order.find("hamburger") != -1) || (order.find("Hamburger") != -1)) {
		string info = "Your order-" + food[0] + ". Waiting time:"  +to_string(timefood[0]) + ". Price:" + to_string(moneyfood[0]) + " hryvnias.";
		send(clientSocket, info.c_str(), info.size(), 0);
		

		 time1 = timefood[0];
		totalTime = totalTime + time1;
		 money1 = moneyfood[0];
		totalMoney = totalMoney + money1;

		Sleep(5000);
		string ready = "Take order-" + food[0]+".";
		send(clientSocket, ready.c_str(), ready.size(), 0);
	}


	if ((order.find("sprite") != -1) || (order.find("Sprite") != -1)) {

		string info = "Your order-" + food[1] + ". Waiting time:" + to_string(timefood[1]) + ". Price:" + to_string(moneyfood[1]) + " hryvnias.";
		send(clientSocket, info.c_str(), info.size(), 0);

		time2 = timefood[1];
		totalTime = totalTime + time2;
		 money2 = moneyfood[1];
		totalMoney = totalMoney + money2;

		Sleep(1000);
		string ready = "Take order-" + food[1] + ".";
		send(clientSocket, ready.c_str(), ready.size(), 0);
	}


	if ((order.find("free potato") != -1) || (order.find("Free potato") != -1) || (order.find("free Potato") != -1)) {

		string info = "Your order-" + food[2] + ". Waiting time:" + to_string(timefood[2]) + ". Price:" + to_string(moneyfood[2]) + " hryvnias.";
		send(clientSocket, info.c_str(), info.size(), 0);

		 time3 = timefood[2];
		totalTime = totalTime + time3;
		 money3 = moneyfood[2];
		totalMoney = totalMoney + money3;

		Sleep(3000);
		string ready = "Take order-" + food[2] + ".";
		send(clientSocket, ready.c_str(), ready.size(), 0);
	}


	if ((order.find("ice cream") != -1) || (order.find("Ice cream") != -1) || (order.find("ice Cream") != -1)) {

		string info = "Your order-" + food[3] + ". Waiting time:" + to_string(timefood[3]) + ". Price:" + to_string(moneyfood[3]) + " hryvnias.";
		send(clientSocket, info.c_str(), info.size(), 0);

		 time4 = timefood[3];
		totalTime = totalTime + time4;
		 money4 = moneyfood[3];
		totalMoney = totalMoney + money4;

		Sleep(2000);
		string ready = "Take order-" + food[3] + ".";
		send(clientSocket, ready.c_str(), ready.size(), 0);
	}

	
	 string response = "Waiting time: " + to_string(totalTime) + " seconds. Cost:" + to_string(totalMoney)+" hryvnias.";
	 send(clientSocket, response.c_str(), response.size(), 0);

}





void AddRequestToQueue(SOCKET clientSocket, char* clientMessage) {
	if (numRequests < MAX_CLIENTS) {

		requestArray[numRequests].clientSocket = clientSocket;// ���������  � ������ �������� requestArray
		requestArray[numRequests].order = clientMessage;// ���������  � ������ �������� requestArray
		numRequests++;
		if (clientMessage == "off")
		{
			WSACleanup();
		}

		
	}
	else {
		cout << "Queue is full." << endl;
	}
}

void ProcessRequests() {
	for (int i = 0; i < numRequests; i++) {
		SOCKET clientSocket = requestArray[i].clientSocket; // �������� ����� ������� �� �������
		string order = requestArray[i].order; // �������� ����� ������� �� �������


		Sleep(9000);
		string response = "Processing  order: " + order;
		send(clientSocket, response.c_str(), response.size(), 0);


		char buf[255];
		strcpy(buf, order.c_str());
		Order(clientSocket, buf);


	}
	numRequests = 0; // ����� ���������� �������� ����� ��������� ����
}

////////////////////////////////////////////////////////////////////






int main() {
	system("title Server");

	puts("Start server... DONE.");
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code: %d", WSAGetLastError());
		return 1;
	}

	// create a socket
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket: %d", WSAGetLastError());
		return 2;
	}
	// puts("Create socket... DONE.");

	// prepare the sockaddr_in structure
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	// bind socket
	if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		return 3;
	}

	// puts("Bind socket... DONE.");

	// ������� �������� ����������
	listen(server_socket, MAX_CLIENTS);

	// ������� � �������� ����������
	puts("Server is waiting for incoming connections...\nPlease, start one or more client-side app.");

	// ������ ������ ��������� ������, ��� ����� ������
	// ����� ������������ �������
	// fd means "file descriptors"
	fd_set readfds; // https://docs.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-fd_set
	SOCKET client_socket[MAX_CLIENTS] = {};

	while (true) {
		// �������� ����� fdset
		FD_ZERO(&readfds);

		// �������� ������� ����� � fdset
		FD_SET(server_socket, &readfds);

		// �������� �������� ������ � fdset
		for (int i = 0; i < MAX_CLIENTS; i++) 
		{
			SOCKET s = client_socket[i];
			if (s > 0) {
				FD_SET(s, &readfds);
			}
		}

		// ��������� ���������� �� ����� �� �������, ����-��� ����� NULL, ������� ����� ����������
		if (select(0, &readfds, NULL, NULL, NULL) == SOCKET_ERROR) {
			printf("select function call failed with error code : %d", WSAGetLastError());
			return 4;
		}

		// ���� ���-�� ��������� �� ������-������, �� ��� �������� ����������
		SOCKET new_socket; // ����� ���������� �����
		sockaddr_in address;
		int addrlen = sizeof(sockaddr_in);
		if (FD_ISSET(server_socket, &readfds)) {
			if ((new_socket = accept(server_socket, (sockaddr*)&address, &addrlen)) < 0) {
				perror("accept function error");
				return 5;
			}
			
			
			for (int i = 0; i < history.size(); i++)
			{
				cout << history[i] << "\n";
				send(new_socket, history[i].c_str(), history[i].size(), 0);
			}
			
			

			// ������������� ��������� ������� � ������ ������ - ������������ � �������� �������� � ���������
			printf("New connection, socket fd is %d, ip is: %s, port: %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

			// �������� ����� ����� � ������ �������
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (client_socket[i] == 0) {
					client_socket[i] = new_socket;
					printf("Adding to list of sockets at index %d\n", i);
					break;
				}
			}
		}

		// ���� �����-�� �� ���������� ������� ���������� ���-��
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			SOCKET s = client_socket[i];
			// ���� ������ ������������ � ������� ������
			if (FD_ISSET(s, &readfds))
			{
				// �������� ��������� �������
				getpeername(s, (sockaddr*)&address, (int*)&addrlen);

				// ���������, ���� �� ��� �� ��������, � ����� ���������� �������� ���������
				// recv �� �������� ������� ���������� � ����� ������ (� �� ����� ��� printf %s ������������, ��� �� ����)

				char client_message[DEFAULT_BUFLEN];

				int client_message_length = recv(s, client_message, DEFAULT_BUFLEN, 0);
				client_message[client_message_length] = '\0';

				string check_exit = client_message;
				if (check_exit == "off")
				{
					cout << "Client #" << i << " is off\n";
					client_socket[i] = 0;
					closesocket(client_socket[i]);
				}
				else
				{
					AddRequestToQueue(s, client_message);
					continue;
				}
				
				//string temp = client_message;
				// temp += "\n";
			
				//history.push_back(temp);

						/*	for (int i = 0; i < MAX_CLIENTS; i++) {
						if (client_socket[i] != 0) {
							send(client_socket[i], client_message, client_message_length, 0);
						}
					}*/
				
			}
		}
		ProcessRequests();
		Sleep(1000);
		///////////////////////
	}

	WSACleanup();
}