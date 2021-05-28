#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define BUFFLEN 65535
#define PORT 19973

void die(const char *s){
	perror(s);
	exit(1);
}

int main(void){
	struct sockaddr_in si_me, si_other;
	int s, i, recv_len;
	socklen_t slen = sizeof(si_other);
	char buf[BUFFLEN];

	if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket create failed");
	}
	memset((char*)&si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(s, (struct sockaddr*)&si_me,sizeof(si_me)) == -1)
	{
		die("bind error");
	}

	while(true){
		int rec_len = recvfrom(s, buf, BUFFLEN, 0, (struct sockaddr *)&si_other, &slen);
		buf[rec_len] = '\0';
		cout<<buf<<endl;
	}
	return 0;
}
