#include "TouchPadThread.h"

DWORD WINAPI TouchPadThread(LPVOID) {

	//防止由于某些意外启动多个线程
	CreateMutexA(0, true, "TouchThread");
	if (ERROR_ALREADY_EXISTS == GetLastError()) {
		return 0;
	}

	printf("Touchpad server start\n");
	//创建UDP Server
	SOCKET serversoc;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
	int server_len = sizeof(serveraddr);
	char szBuf[65535];
	int result;

	//命名协议，IP，端口
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(19973);
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	//创建socket
	serversoc = socket(AF_INET, SOCK_DGRAM, 0);


	//绑定socket
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
		//接收数据
		result = recvfrom(serversoc, szBuf, 65535, 0, 0, 0);
		char temp[2][64];
		int s = 0, i = 0, ii = 2,iii=3;
		char temp1[64];
		if (result >= 0)
		{
			szBuf[result] = 0;
			//printf("接收数据为:  %s \n", szBuf);
			switch (szBuf[0]) {
				//鼠标左键
				case 'L':
					//单击
					if (szBuf[1] == 'c') {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}//双击
					else if (szBuf[1] == 'C') {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						Sleep(20);
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}//按下
					else  if (szBuf[1] == 'D') {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					}//抬起
					else  if (szBuf[1] == 'U') {
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
					break;
				case 'R':
					//单击
					if (szBuf[1] == 'C') {
						mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
					}//按下
					else  if (szBuf[1] == 'D') {
						mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					}//抬起
					else  if (szBuf[1] == 'U') {
						mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
					}
					break;
				case 'M':
					//鼠标移动 分解移动值
					s = 0, i = 0, ii = 2;
					while (szBuf[ii] != 0) {
						if (szBuf[ii] == '#')  temp[s][i]=0,s++,ii++,i=0;
						temp[s][i++] = szBuf[ii++];
					}
					temp[s][i] = 0, s = 0;
					mouse_event(MOUSEEVENTF_MOVE, atoi(temp[0]), atoi(temp[1]), 0, 0);
					break;
				case 'S':
					//滚动条 确定垂直水平 然后分解值
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