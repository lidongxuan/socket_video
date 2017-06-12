#include <stdio.h>
#include <string>
#include <iostream>
#include <Winsock2.h>
#include <opencv2/opencv.hpp>
#include <vector> 
#pragma comment(lib,"ws2_32.lib")

using namespace cv;
using namespace std;

void main()
{
	WSADATA wsaData;
	SOCKET sockClient;//客户端Socket
	SOCKADDR_IN addrServer;//服务端地址
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//新建客户端socket
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//定义要连接的服务端地址
	addrServer.sin_addr.S_un.S_addr = inet_addr("10.106.20.111");//目标IP(10.106.20.74是回送地址)
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(8010);//连接端口
	//连接到服务端
	connect(sockClient, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

	Mat image;
	VideoCapture capture(0);
	vector<uchar> data_encode;

	while (1)
	{
		if (!capture.read(image)) 
			break;
		imencode(".jpg", image, data_encode);
		int len_encode = data_encode.size();
		string len = to_string(len_encode);
		int length = len.length();
		for (int i = 0; i < 16 - length; i++)
		{
			len = len + " ";
		}
		//发送数据
		send(sockClient, len.c_str(), strlen(len.c_str()), 0);
		char send_char[1];
		for (int i = 0; i < len_encode; i++)
		{
			send_char[0] = data_encode[i];
			send(sockClient, send_char, 1, 0);
		}
		//接收返回信息
		char recvBuf[32];
		if(recv(sockClient, recvBuf, 32, 0))
			printf("%s\n", recvBuf);
	}
	closesocket(sockClient);
	WSACleanup();
}