#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUFFLEN 65535
#define PORT 19973
#define MOUSEEVENTF_MOVE        0x0001 /* 鼠标移动 */
#define MOUSEEVENTF_LEFTDOWN    0x0002 /* 左键按下 */
#define MOUSEEVENTF_LEFTUP      0x0004 /* 左键弹起 */
#define MOUSEEVENTF_RIGHTDOWN   0x0008 /* 右键按下 */
#define MOUSEEVENTF_RIGHTUP     0x0010 /* 右键弹起 */

#define MOUSEEVENTF_WHEEL       0x0800     
#define MOUSEEVENTF_HWHEEL      0x01000 /* 滚轮按钮事件*/  

#define MOUSEEVENTF_MIDDLEDOWN  0x0020 /* middle button down */
#define MOUSEEVENTF_MIDDLEUP    0x0040 /* middle button up */
#define MOUSEEVENTF_XDOWN       0x0080 /* x button down */
#define MOUSEEVENTF_XUP         0x0100 /* x button down */

#define WHEEL_DELTA 120
void die(const char *s)
{
	perror(s);
	exit(1);
}

void mouse_event(unsigned dwFlag, int dx, int dy, int dwData, void* dwExtraInfo){
	cout<<dwFlag<<" "<<dx<<" "<<dy<<" "<<dwData<<" "<<dwExtraInfo<<endl;
}

void Sleep(unsigned a){

}

int main(void)
{
	struct sockaddr_in si_me, si_other;
	int s, i, recv_len;
	socklen_t slen = sizeof(si_other);
	char szBuf[BUFFLEN];

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket create failed");
	}
	memset((char *)&si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) == -1)
	{
		die("bind error");
	}

	while (true)
	{
		int result = recvfrom(s, szBuf, BUFFLEN, 0, (struct sockaddr *)&si_other, &slen);
		char temp[2][64];
		int s = 0, i = 0, ii = 2, iii = 3;
		char temp1[64];
		if (result >= 0)
		{
			szBuf[result] = 0;
			//printf("接收数据为:  %s \n", szBuf);
			switch (szBuf[0])
			{
			//鼠标左键
			case 'L':
				//单击
				if (szBuf[1] == 'c')
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				} //双击
				else if (szBuf[1] == 'C')
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					Sleep(20);
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				} //按下
				else if (szBuf[1] == 'D')
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				} //抬起
				else if (szBuf[1] == 'U')
				{
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
				break;
			case 'R':
				//单击
				if (szBuf[1] == 'C')
				{
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				} //按下
				else if (szBuf[1] == 'D')
				{
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
				} //抬起
				else if (szBuf[1] == 'U')
				{
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				}
				break;
			case 'M':
				//鼠标移动 分解移动值
				s = 0, i = 0, ii = 2;
				while (szBuf[ii] != 0)
				{
					if (szBuf[ii] == '#')
						temp[s][i] = 0, s++, ii++, i = 0;
					temp[s][i++] = szBuf[ii++];
				}
				temp[s][i] = 0, s = 0;
				mouse_event(MOUSEEVENTF_MOVE, atoi(temp[0]), atoi(temp[1]), 0, 0);
				break;
			case 'S':
				//滚动条 确定垂直水平 然后分解值
				if (szBuf[1] == 'H')
				{
					iii = 3, i = 0;
					while (szBuf[iii])
					{
						temp1[i++] = szBuf[iii++];
					}
					temp1[i] = 0;
					mouse_event(MOUSEEVENTF_HWHEEL, 0, 0, atoi(temp1) * WHEEL_DELTA, 0);
				}
				else if (szBuf[1] == 'V')
				{
					iii = 3, i = 0;
					while (szBuf[iii])
					{
						temp1[i++] = szBuf[iii++];
					}
					temp1[i] = 0;
					mouse_event(MOUSEEVENTF_WHEEL, 0, 0, atoi(temp1) * WHEEL_DELTA, 0);
				}
				break;
			case 'A':
				break;
			}
		}
		else
		{
			printf("touchpad socket error");
		}
	}
	return 0;
}
