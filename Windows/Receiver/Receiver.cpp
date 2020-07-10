#include <iostream>

#include "BroadPCThread.h"
#include "PostPCMThread.h"
#include "TouchPadThread.h"

using namespace std;

BOOL bAudioDone = FALSE;

DWORD WINAPI KeepAliveThread(LPVOID){
	SOCKET sockKeepAlive = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockKeepAlive == SOCKET_ERROR) {
		cout << "Create socket failed,sockkeepalive" << WSAGetLastError() << endl;
	}
	sockaddr_in addrKeepAlive;
	addrKeepAlive.sin_family = AF_INET;
	addrKeepAlive.sin_port = htons(19972);
	addrKeepAlive.sin_addr.S_un.S_addr = INADDR_ANY;
	auto r = bind(sockKeepAlive, (sockaddr*)&addrKeepAlive, sizeof(addrKeepAlive));
	if (SOCKET_ERROR == r) {
		cout << "Bind socket failed,sockkeepalive" << WSAGetLastError() << endl;
	}
	r = listen(sockKeepAlive, 10);
	if (SOCKET_ERROR == r) {
		cout << "Listten socket failed,sockkeepalive" << WSAGetLastError() << endl;
	}
	cout << "Listen on 19972 on all interface" << endl;
	SOCKET sClient;
	sockaddr_in addrClient;
	int addrLen = sizeof(addrClient);
	char bBuff[1024];
	//Only handle on connection
	while ((sClient = accept(sockKeepAlive,(sockaddr*)&addrClient,&addrLen))!=INVALID_SOCKET) {
		//cout << "Accept socket!" << endl;
		int l = recv(sClient, bBuff, 1024, 0);
		if (-1 == l) {
			cout << "ERROR" << WSAGetLastError() << endl;
			continue;
		}
		bBuff[l] = 0;
		//cout << bBuff << endl;
		//if touchpad service is true start thouchpad service
		if (bBuff[l - 2] == 'u') {
			CreateThread(0, 0, TouchPadThread, 0, 0, 0);
		}
		l = recv(sClient, bBuff, 1024, 0);		if (-1 == l) {
			cout << "ERROR" << WSAGetLastError() << endl;
			continue;
		}
		bBuff[l] = 0;
		//if audio service is true
		if (bBuff[l - 2] == 'u') {
			CreateThread(NULL, 0, PCMThread, inet_ntoa(addrClient.sin_addr), 0, 0);
		}
		cout << bBuff << endl;
		while (true) {
			l = recv(sClient, bBuff, 1024, 0);
			if (l == 0 || l < 0) {
				closesocket(sClient);
				cout << "On client closed" << endl;
				break;
			}
		}
	}
	return 0;
}

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	CreateThread(NULL, 0, BroadPCThread, 0, 0, 0);
	CreateThread(NULL, 0, KeepAliveThread, 0, 0, 0);
	while (1);
}
