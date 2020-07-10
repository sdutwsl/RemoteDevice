#include "TouchPadThread.h"

DWORD WINAPI TouchPadThread(LPVOID) {

	//��ֹ����ĳЩ������������߳�
	CreateMutexA(0, true, "TouchThread");
	if (ERROR_ALREADY_EXISTS == GetLastError()) {
		return 0;
	}

	printf("Touchpad server start\n");
	//����UDP Server
	SOCKET serversoc;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
	int server_len = sizeof(serveraddr);
	char szBuf[65535];
	int result;

	//����Э�飬IP���˿�
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(19973);
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	//����socket
	serversoc = socket(AF_INET, SOCK_DGRAM, 0);


	//��socket
	result = bind(serversoc, (SOCKADDR*)&serveraddr, server_len);
	if (result == SOCKET_ERROR)
	{
		printf("socket error on Touchpad thread!\n");
		closesocket(serversoc);
		return -1;
	}

	printf("Touchpad server is running.....\n");

	while (1)
	{
		//��������
		result = recvfrom(serversoc, szBuf, 65535, 0, 0, 0);
		char temp[2][64];
		int s = 0, i = 0, ii = 2,iii=3;
		char temp1[64];
		if (result >= 0)
		{
			szBuf[result] = 0;
			//printf("��������Ϊ:  %s \n", szBuf);
			switch (szBuf[0]) {
				//������
				case 'L':
					//����
					if (szBuf[1] == 'c') {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}//˫��
					else if (szBuf[1] == 'C') {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						Sleep(20);
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}//����
					else  if (szBuf[1] == 'D') {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					}//̧��
					else  if (szBuf[1] == 'U') {
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
					break;
				case 'R':
					//����
					if (szBuf[1] == 'C') {
						mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
					}//����
					else  if (szBuf[1] == 'D') {
						mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					}//̧��
					else  if (szBuf[1] == 'U') {
						mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
					}
					break;
				case 'M':
					//����ƶ� �ֽ��ƶ�ֵ
					s = 0, i = 0, ii = 2;
					while (szBuf[ii] != 0) {
						if (szBuf[ii] == '#')  temp[s][i]=0,s++,ii++,i=0;
						temp[s][i++] = szBuf[ii++];
					}
					temp[s][i] = 0, s = 0;
					mouse_event(MOUSEEVENTF_MOVE, atoi(temp[0]), atoi(temp[1]), 0, 0);
					break;
				case 'S':
					//������ ȷ����ֱˮƽ Ȼ��ֽ�ֵ
					if (szBuf[1] == 'H') {
						iii = 3,i=0;
						while (szBuf[iii]) {
							temp1[i++] = szBuf[iii++];
						}
						temp1[i] = 0;
						mouse_event(MOUSEEVENTF_HWHEEL, 0, 0, atoi(temp1)*WHEEL_DELTA, 0);
					}
					else if (szBuf[1] == 'V') {
						iii = 3, i = 0;
						while (szBuf[iii]) {
							temp1[i++] = szBuf[iii++];
						}
						temp1[i] = 0;
						mouse_event(MOUSEEVENTF_WHEEL, 0, 0, atoi(temp1)* WHEEL_DELTA, 0);
					}
					break;
				case 'A':
					break;
			}
		}
		else {
			printf("touchpad socket error:%d\n", WSAGetLastError());
		}
	}

	closesocket(serversoc);

	return 0;
}