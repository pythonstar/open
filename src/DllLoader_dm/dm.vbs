'dm.dll will be found in wscript.exe process

Set dm = CreateObject("dm.dmsoft")
ver = dm.Ver()
If len(ver) = 0 Then
    MessageBox "��������ʧ��,���ϵͳ�Ƿ������vbs�ű�Ȩ��"
    EndScript
Else
    MsgBox dm.Ver(), 0, "��ע��Ĵ�Į����汾Ϊ��"
End If