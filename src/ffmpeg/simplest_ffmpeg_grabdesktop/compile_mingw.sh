#! /bin/sh
#��򵥵Ļ���FFmpeg��AVDevice���ӣ���Ļ¼�ƣ� ----MinGW�����б���
#Simplest FFmpeg Device (Screen Capture) ----Compile in MinGW 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
g++ simplest_ffmpeg_grabdesktop.cpp -g -o simplest_ffmpeg_grabdesktop.exe \
-I /usr/local/include -L /usr/local/lib \
-lmingw32 -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lavdevice -lswscale
