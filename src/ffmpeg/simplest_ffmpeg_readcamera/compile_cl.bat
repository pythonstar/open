::��򵥵Ļ���FFmpeg��AVDevice���ӣ���ȡ����ͷ��----�����б���
::Simplest FFmpeg Device (Read Camera)----Compile in Cmd 
::
::������ Lei Xiaohua
::leixiaohua1020@126.com
::�й���ý��ѧ/���ֵ��Ӽ���
::Communication University of China / Digital TV Technology
::http://blog.csdn.net/leixiaohua1020
::
::VS2010 Environment
call "D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
::include
@set INCLUDE=include;%INCLUDE%
::lib
@set LIB=lib;%LIB%
::compile and link
cl simplest_ffmpeg_readcamera.cpp /MD /link SDL.lib SDLmain.lib avcodec.lib ^
avformat.lib avutil.lib avdevice.lib avfilter.lib postproc.lib swresample.lib swscale.lib ^
/SUBSYSTEM:WINDOWS /OPT:NOREF
exit