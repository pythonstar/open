#! /bin/sh
#��򵥵Ļ���FFmpeg��AVDevice���ӣ���ȡ����ͷ��----�����б���
#Simplest FFmpeg Device (Read Camera)----Compile in Shell 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_ffmpeg_readcamera.cpp -g -o simplest_ffmpeg_readcamera.out \
-framework Cocoa -I /usr/local/include -L /usr/local/lib -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lavdevice -lswscale
