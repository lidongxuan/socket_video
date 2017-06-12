#include <stdio.h>
#include <Winsock2.h>
#include <opencv2/opencv.hpp>
#include <vector> 
#pragma comment(lib,"ws2_32.lib")

using namespace cv;
using namespace std;

void main()
{
	WSADATA wsaData;
	SOCKET sockServer;
	SOCKADDR_IN addrServer;
	SOCKET conn;
	SOCKADDR_IN addr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建Socket  
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	//准备通信地址  
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(8010);
	//绑定  
	bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
	//监听  
	listen(sockServer, 5);
	printf("Waiting for images...\n");

	int len = sizeof(SOCKADDR);
	//监听连接  
	conn = accept(sockServer, (SOCKADDR*)&addr, &len);

	char recvBuf[16];
	char recvBuf_1[1];
	Mat img_decode;
	vector<uchar> data;

	while (1)
	{
		if (recv(conn, recvBuf, 16, 0))
		{
			for (int i = 0; i < 16; i++)
			{
				if (recvBuf[i]<'0' || recvBuf[i]>'9') recvBuf[i] = ' ';
			}
			data.resize(atoi(recvBuf));
			for (int i = 0; i < atoi(recvBuf); i++)
			{
				recv(conn, recvBuf_1, 1, 0);
				data[i] = recvBuf_1[0];
			}
			printf("Image recieved successfully!\n");
			send(conn, "Server has recieved messages!", 29, 0);
			img_decode = imdecode(data, CV_LOAD_IMAGE_COLOR);
			imshow("server", img_decode);
			if (waitKey(30) == 27) break;
		}
	}
	closesocket(conn);
	WSACleanup();
}