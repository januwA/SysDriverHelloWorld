#pragma once

#include <ntifs.h>
#include <ntddk.h>

// ÐÅºÅ
#define IO_GET_CLIENTADDRESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x666, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x667, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_WRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x668, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Êý¾Ý
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

NTSTATUS CreateCall(_In_ struct _DEVICE_OBJECT* DeviceObject, _Inout_ struct _IRP* Irp);
NTSTATUS CloseCall(_In_ struct _DEVICE_OBJECT* DeviceObject, _Inout_ struct _IRP* Irp);
NTSTATUS IoControl(_In_ struct _DEVICE_OBJECT* DeviceObject, _Inout_ struct _IRP* Irp);
