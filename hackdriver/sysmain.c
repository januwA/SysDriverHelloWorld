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
	// �⽫��ӡ�ܶ���Ϣ
	// mDebug("ImageLoad: %ls \n", FullImageName->Buffer);

	// ���ָ����DLL�Ƿ񱻼���
	if (wcsstr(FullImageName->Buffer, L"\\EmptyDll\\Release\\EmptyDll.dll"))
	{
		// DLL�������ַ
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

	// ÿ����Image�����أ��ͻ�֪ͨ�ص�
	PsSetLoadImageNotifyRoutine(ImageLoadCallback);

	// ���ұߵ��ַ���ʼ�������
	RtlInitUnicodeString(&dev, L"\\Device\\hackdriver");
	RtlInitUnicodeString(&dos, L"\\DosDevices\\hackdriver");

	// ����һ����������ʹ�õ��豸����
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

	// ������һ���豸�������ƺ͸��豸���û���������֮��ķ�������
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

	// �������������ж��ǰɾ����ע������лص�
	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);

	IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(pDriverObject->DeviceObject);

	return STATUS_SUCCESS;
}
