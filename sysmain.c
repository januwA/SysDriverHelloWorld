#pragma warning (disable : 4100 4047 4024)

#include "sysmain.h"
#include "messages.h"
#include "data.h"
#include "communication.h"


PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(
	PUNICODE_STRING FullImageName,
	HANDLE ProcessId,
	PIMAGE_INFO ImageInfo
)
{
	// 这将打印很多信息
	// mDebug("ImageLoad: %ls \n", FullImageName->Buffer);

	// 检查指定的DLL是否被加载
	if (wcsstr(FullImageName->Buffer, L"\\EmptyDll\\Release\\EmptyDll.dll"))
	{
		// DLL的虚拟基址
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/ntddk/ns-ntddk-_image_info
		CientDLLAddress = ImageInfo->ImageBase;
		mDebug("Inject ProcessID: %d \n", ProcessId);
		mDebug("Inject ClientDLLAddress: %x \n", CientDLLAddress);
	}

	return STATUS_SUCCESS;
}


// start
// https://docs.microsoft.com/en-us/windows-hardware/drivers/wdf/driverentry-for-kmdf-drivers
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRrgister)
{
	mDebug("driver start...\n");

	// 每当有Image被加载，就会通知回调
	PsSetLoadImageNotifyRoutine(ImageLoadCallback);

	// 将右边的字符初始化到左边
	RtlInitUnicodeString(&dev, L"\\Device\\hackdriver");
	RtlInitUnicodeString(&dos, L"\\DosDevices\\hackdriver");

	// 创建一个驱动程序使用的设备对象
	if (IoCreateDevice(
		pDriverObject,
		0,
		&dev,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&pDeviceObject
	) == STATUS_SUCCESS)
	{
		mDebug("---IoCreateDevice ok---");
	}
	else
	{
		mDebug("---IoCreateDevice error---");
	}

	// 设置了一个设备对象名称和该设备的用户可视名称之间的符号链接
	IoCreateSymbolicLink(&dos, &dev);

	// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/ns-wdm-_driver_object
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;
	pDriverObject->DriverUnload = UnloadDriver;

	pDeviceObject->Flags |= DO_DIRECT_IO;
	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	return STATUS_SUCCESS;
}

// stop
NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	mDebug("driver stop...\n");

	// 驱动程序必须在卸载前删除其注册的所有回调
	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);

	IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(pDriverObject->DeviceObject);

	return STATUS_SUCCESS;
}
