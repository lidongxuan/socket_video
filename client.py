#!/usr/bin/python
#-*-coding:utf-8 -*-
import socket
import cv2
import numpy

# socket.AF_INET用于服务器与服务器之间的网络通信
# socket.SOCK_STREAM代表基于TCP的流式socket通信
sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
# 连接服务端
address_server = ('10.106.20.111', 8010)
sock.connect(address_server)

# 从摄像头采集图像
capture = cv2.VideoCapture(0)
ret, frame = capture.read()
encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90] #设置编码参数

while ret: 
	# 首先对图片进行编码，因为socket不支持直接发送图片
    result, imgencode = cv2.imencode('.jpg', frame)
    data = numpy.array(imgencode)
    stringData = data.tostring()
    # 首先发送图片编码后的长度
    sock.send(str(len(stringData)).ljust(16))
    # 然后一个字节一个字节发送编码的内容
    # 如果是python对python那么可以一次性发送，如果发给c++的server则必须分开发因为编码里面有字符串结束标志位，c++会截断
    for i in range (0,len(stringData)):
    	sock.send(stringData[i])
    ret, frame = capture.read()
    #cv2.imshow('CLIENT',frame)
    # if cv2.waitKey(10) == 27:
    #     break
    # 接收server发送的返回信息
    data_r = sock.recv(50)
    print (data_r)

sock.close()
cv2.destroyAllWindows()