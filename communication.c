#include "communication.h"
#include "messages.h"
#include "data.h"

NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	mDebug("CreateCall\n");
	return STATUS_SUCCESS;
}

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	mDebug("CloseCall\n");
	return STATUS_SUCCESS;
}

NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS status = STATUS_UNSUCCESSFUL;
	ULONG byteIO = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	ULONG controlCode = stack->Parameters.DeviceIoControl.IoControlCode;

	if (controlCode == IO_GET_CLIENTADDRESS)
	{
		mDebug("---收到通信---\n");
		PULONG outPut = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*outPut = CientDLLAddress;
		mDebug("ClienAddress requested!\n");
		status = STATUS_SUCCESS;
		byteIO = sizeof(*outPut);
	}
	else
	{
		byteIO = 0;
	}

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = byteIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}
