#include "BroadPCThread.h"

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3
#define BROADCASTPORT 45678

char sAddress[255][64];
UINT uAddindex = 0;

DWORD WINAPI BroadSubThread(LPVOID ipaddress);

DWORD __stdcall BroadPCThread(LPVOID param)
{	//防止由于某些意外启动多个线程
	CreateMutexA(0, true, "BroadThread");
	if (ERROR_ALREADY_EXISTS == GetLastError()) {
		return 0;
	}

	//GetAdaptersBroadcastAddress
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return 1;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return 1;
		}
	}
	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			struct in_addr in;
			printf("%s\n", pAdapter->IpAddressList.IpAddress.String);
			//get broadcast address use ip and ip mask
			in.S_un.S_addr = inet_addr(pAdapter->IpAddressList.IpAddress.String) | ~inet_addr(pAdapter->IpAddressList.IpMask.String);
			strcpy(sAddress[uAddindex], inet_ntoa(in));
			CreateThread(NULL, 0, BroadSubThread,sAddress[uAddindex++], 0, 0);
			pAdapter = pAdapter->Next;
		}
	}
	else {
		printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
	}
	if (pAdapterInfo)
		FREE(pAdapterInfo);
	return 0;
}

DWORD WINAPI BroadSubThread(LPVOID ipaddress) {

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (!strcmp((char*)ipaddress, "255.255.255.255"))
	{
		printf("Limited address\n");
		return 0;
	}
	else {
		printf("%s\n", (char*)ipaddress);
	}
	SOCKET sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	char broadcast = '1';
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
	{
		printf("Error in setting Broadcast option");
		closesocket(sock);
		return 0;
	}
	struct sockaddr_in Recv_addr;
	struct sockaddr_in Sender_addr;
	int len = sizeof(struct sockaddr_in);
	char sendMSG[1204] = "Broadcast message from SLAVE TAG";
	gethostname(sendMSG, 1024);
	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(BROADCASTPORT);
	Recv_addr.sin_addr.s_addr = inet_addr((char*)ipaddress);
	while (true) {
		sendto(sock, sendMSG, strlen(sendMSG) + 1, 0, (sockaddr*)&Recv_addr, sizeof(Recv_addr));
		printf("Send broadcast:%s\n", sendMSG);
		Sleep(1000);
	}
	return 0;
}
