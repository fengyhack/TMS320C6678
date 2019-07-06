#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "D:\Program\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\winapps\sockets.h"
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

typedef unsigned char byte;

const char  *TARGET = "192.168.2.100";
#define PORT  7
#define TIMEOUT 1   /* second(s) */
#define BUFF_UNIT 1024

const char* reply_msg = "HELLO";

#define FD_SELECT 	{ \
	FD_ZERO(&fds); \
    FD_SET(s, &fds);  \
    select(s + 1, &fds, NULL, NULL, &timeout); }

int main(void)
{
	system("title Ethernet0 Transfer Test");
	system("color 2e");
	printf("Remote ( %s : %d )\n", TARGET, PORT);


	char fileName[256] = { 0 };
	printf("Input filename:");
	scanf("%s", fileName);

	FILE* fp_input = fopen(fileName, "rb");
	long size;
	fread(&size, sizeof(long), 1, fp_input);
	
	byte* data = (byte*)malloc(size);
	fread(data, size, 1, fp_input);
	fclose(fp_input);

	int unit, cycle, residue;

	unit = BUFF_UNIT;
	cycle = size / unit;
	residue = size%unit;

	struct in_addr dst;
	inet_pton(AF_INET, TARGET, &dst);
	unsigned short port = PORT;
	socketsStartup();
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = 0;

	const int reply_len = strlen(reply_msg);
	const int saddr_len = sizeof(sockaddr_in);

	bind(s, (const struct sockaddr *) &saddr, saddr_len);
	saddr.sin_addr = dst;
	saddr.sin_port = htons(port);

	struct timeval timeout;
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;

	fd_set fds;
	int i;
	char tmp[8] = { 0 };

	time_t time_start = time(0);

	sendto(s, (const char*)&size, sizeof(long), 0, (const struct sockaddr *) &saddr, saddr_len);
	FD_SELECT
	recv(s, tmp, reply_len, 0);

	byte* ptr = data;
	for (i = 1; i <= cycle; ++i)
	{
		sendto(s, (const char*)ptr, unit, 0, (const struct sockaddr *) &saddr, saddr_len);
		ptr += unit;
		recv(s, tmp, reply_len, 0);
	}
	if (residue > 0)
	{
		sendto(s, (const char*)ptr, residue, 0, (const struct sockaddr *) &saddr, saddr_len);
		recv(s, tmp, reply_len, 0);
	}
	printf("\nFinished transfer, waiting for the reply...\n");

	recv(s, (char*)&unit, sizeof(int), 0);
	sendto(s, reply_msg, reply_len, 0, (const struct sockaddr *) &saddr, saddr_len);
	recv(s, (char*)&cycle, sizeof(int), 0);
	sendto(s, reply_msg, reply_len, 0, (const struct sockaddr *) &saddr, saddr_len);
	recv(s, (char*)&residue, sizeof(int), 0);
	sendto(s, reply_msg, reply_len, 0, (const struct sockaddr *) &saddr, saddr_len);

	ptr = data;
	for (i = 1; i <= cycle; ++i)
	{
		recv(s, (char*)ptr, unit, 0);
		ptr += unit;
		sendto(s, reply_msg, reply_len, 0, (const struct sockaddr *) &saddr, saddr_len);
	}
	if (residue > 0)
	{
		recv(s, (char*)ptr, residue, 0);
		sendto(s, reply_msg, reply_len, 0, (const struct sockaddr *) &saddr, saddr_len);
	}
	printf("\nFinished receiving the result data.\n");
	int elapsed = (int)(time(0) - time_start);
	printf("Time = %d seconds\n", elapsed);

	char sname[256] = { 0 };
	printf("Input dest file name:");
	scanf("%s", sname);

	FILE* out = fopen(sname, "wb");
	fwrite(&size, sizeof(long), 1, out);
	fwrite(data, size, 1, out);
	fflush(out);
	fclose(out);


	closesocket(s);
	socketsShutdown();

	system("pause");
	return 0;
}
