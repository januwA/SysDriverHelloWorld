#pragma once

#define IO_GET_CLIENTADDRESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x666, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#include <Windows.h>

class KernelInterface
{
public:
	HANDLE hDriver;
	KernelInterface(LPCSTR registerPath);
	DWORD getClientAddress();
};
