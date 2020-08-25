#include <iostream>
#include <Windows.h>

// 通信 信号
#define IO_GET_CLIENTADDRESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x666, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x667, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_WRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x668, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// 通信 数据
typedef struct _KERNEL_READ_REQUEST
{
	ULONG processId;
	PVOID address;
	PVOID pBuff;
	ULONG size;
} KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;
typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG processId;
	PVOID address;
	PVOID pBuff;
	ULONG size;
} KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;


class KernelTool
{
public:
	HANDLE hDriver;
	KernelTool(const char* fileName)
	{
		hDriver = CreateFileA(fileName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			0, OPEN_EXISTING, 0, 0);
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			printf("CreateFileA error.");
		}
	}

	ULONG getDllImagseBase()
	{
		ULONG Addresss;
		DWORD Bytes;
		DeviceIoControl(hDriver, IO_GET_CLIENTADDRESS, &Addresss, sizeof(Addresss),
			&Addresss, sizeof(Addresss), &Bytes, 0);
		return Addresss;
	}

	bool ReadMemory(size_t processId, PVOID address, PVOID data, size_t size)
	{
		KERNEL_READ_REQUEST readRequest;
		readRequest.processId = processId;
		readRequest.address = address;
		readRequest.pBuff = data;
		readRequest.size = size;
		DWORD Bytes;
		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &readRequest, sizeof(readRequest),
			&readRequest, sizeof(readRequest), &Bytes, 0))
		{
			return true;
		}
		return false;
	}

	bool writeMemory(size_t processId, PVOID address, PVOID data, size_t size)
	{
		KERNEL_WRITE_REQUEST writeRequest;
		writeRequest.processId = processId;
		writeRequest.address = address;
		writeRequest.pBuff = data;
		writeRequest.size = size;
		DWORD Bytes;
		if (DeviceIoControl(hDriver, IO_WRITE_REQUEST, &writeRequest, sizeof(writeRequest),
			0, 0, &Bytes, 0))
		{
			return true;
		}
		return false;
	}
};


int main()
{
	KernelTool kt("\\\\.\\ajanuwdriver");
	if (!kt.hDriver) return 0;


	PVOID hpAddress = (PVOID)0x4B3724;
	uintptr_t processId = 3144;

	DWORD curHP;
	if (kt.ReadMemory(processId, hpAddress, &curHP, sizeof(DWORD)))
	{
		printf("当前hp: %d\n", curHP);
	}

	DWORD newHp = 100;

	if (kt.writeMemory(processId, hpAddress, &newHp, sizeof(DWORD)))
	{
		printf("修改hp ok.");
	}

	return 0;
}
