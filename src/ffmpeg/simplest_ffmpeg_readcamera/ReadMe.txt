 ��򵥵Ļ���FFmpeg��AVDevice���ӣ���ȡ����ͷ��
 Simplest FFmpeg Device (Read Camera)

 ������ Lei Xiaohua
 leixiaohua1020@126.com
 �й���ý��ѧ/���ֵ��Ӽ���
 Communication University of China / Digital TV Technology
 http://blog.csdn.net/leixiaohua1020

 ������ʵ���˱�������ͷ���ݵĻ�ȡ�������ʾ���ǻ���FFmpeg
 ��libavdevice�����򵥵����ӡ�ͨ�������ӣ�����ѧϰFFmpeg��
 libavdevice����ʹ�÷�����
 ��������Windows�¿���ʹ��2�ַ�ʽ��ȡ����ͷ���ݣ�
  1.VFW: Video for Windows ��Ļ��׽�豸��ע������URL���豸����ţ�
          ��0��9��
  2.dshow: ʹ��Directshow��ע�����߻����ϵ�����ͷ�豸������
         ��Integrated Camera����ʹ�õ�ʱ����Ҫ�ĳ��Լ�����������ͷ��
          �������ơ�
 ��Linux�������ʹ��video4linux2��ȡ����ͷ�豸��

 This software read data from Computer's Camera and play it.
 It's the simplest example about usage of FFmpeg's libavdevice Library. 
 It's suiltable for the beginner of FFmpeg.
 This software support 2 methods to read camera in Microsoft Windows:
  1.gdigrab: VfW (Video for Windows) capture input device.
             The filename passed as input is the capture driver number,
             ranging from 0 to 9.
  2.dshow: Use Directshow. Camera's name in author's computer is 
             "Integrated Camera".
 It use video4linux2 to read Camera in Linux.