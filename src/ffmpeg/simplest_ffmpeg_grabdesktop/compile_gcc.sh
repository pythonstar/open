#! /bin/sh
#��򵥵Ļ���FFmpeg��AVDevice���ӣ���Ļ¼�ƣ�----�����б���
#Simplest FFmpeg Device (Screen Capture)----Compile in Shell 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_ffmpeg_grabdesktop.cpp -g -o simplest_ffmpeg_grabdesktop.out \
-I /usr/local/include -L /usr/local/lib -lSDLmain -lSDL -lavformat -lavcodec -lavutil -lavdevice -lswscale
