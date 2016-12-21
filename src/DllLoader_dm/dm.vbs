'dm.dll will be found in wscript.exe process

Set dm = CreateObject("dm.dmsoft")
ver = dm.Ver()
If len(ver) = 0 Then
    MessageBox "创建对象失败,检查系统是否禁用了vbs脚本权限"
    EndScript
Else
    MsgBox dm.Ver(), 0, "已注册的大漠插件版本为："
End If