#include <iostream>
#include "kernelinterface.h"

KernelInterface::KernelInterface(LPCSTR registerPath)
{
	hDriver = CreateFileA(
		registerPath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		0,
		0
	);

	if (hDriver == INVALID_HANDLE_VALUE)
	{
		// https://stackoverflow.com/questions/54832148/calling-wdf-driver-from-c
		printf_s("CreateFileW error: %d\n", GetLastError());
	}
};

DWORD KernelInterface::getClientAddress()
{
	if (hDriver == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	ULONG Addresss;
	DWORD Bytes;

	// 将控制代码直接发送到指定的设备驱动程序，使相应的设备执行相应的操作
	if (DeviceIoControl(hDriver, IO_GET_CLIENTADDRESS, &Addresss, sizeof(Addresss), &Addresss, sizeof(Addresss), &Bytes, 0))
	{
		return Addresss;
	}
	else
	{
		printf_s("通信失败。");
	}
	return 0;
};