# SysDriverHelloWorld

内核模式与用户模式通信

- [内核hack video](https://www.youtube.com/watch?v=9h1FsOISwX0&list=PLt9cUwGw6CYHWuMtGHSgdLUaTo7aa2lEr)
- [如何进入测试模式](https://zhuanlan.zhihu.com/p/34743309)
- [DebugView下载](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview)
- [下载WDK](https://docs.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)


```
// 注册服务
C:\WINDOWS\system32>sc create hackdriver type=kernel binpath="C:\Users\ajanuw\Desktop\hack\hackdriver\x64\Release\hackdriver.sys"
[SC] CreateService 成功

// 执行前，需要将系统改为测试模式
// 执行后重启电脑，将会在屏幕右下角看到进入测试模式
C:\WINDOWS\system32>bcdedit /set testsigning on
操作成功完成。

// 打开 DebugView
// 打开 capture->capture kernel选项
// 执行下面的命令将会看到"my first  driver."信息
C:\WINDOWS\system32>sc start hackdriver

// 会看到"unload driver"信息
C:\WINDOWS\system32>sc stop hackdriver

// 查看服务状态
C:\WINDOWS\system32>sc query hackdriver

// (从注册表中)删除服务
// 删除注册表regedit：HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services
C:\WINDOWS\system32>sc delete hackdriver
```