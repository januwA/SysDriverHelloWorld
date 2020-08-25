#pragma warning(disable: 4100 4047 4022)

#include "communication.h"
#include "data.h"
#include "memory.h"

NTSTATUS CreateCall(_In_ struct _DEVICE_OBJECT* DeviceObject, _Inout_ struct _IRP* Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	DbgPrintEx(0, 0, "CreateCall\n");
	return STATUS_SUCCESS;
}

NTSTATUS CloseCall(_In_ struct _DEVICE_OBJECT* DeviceObject, _Inout_ struct _IRP* Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	DbgPrintEx(0, 0, "CloseCall\n");
	return STATUS_SUCCESS;
}

NTSTATUS IoControl(_In_ struct _DEVICE_OBJECT* DeviceObject, _Inout_ struct _IRP* Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS status = STATUS_UNSUCCESSFUL;
	ULONG byteIO = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	ULONG controlCode = stack->Parameters.DeviceIoControl.IoControlCode;

	if (controlCode == IO_GET_CLIENTADDRESS)
	{
		PULONG outPut = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*outPut = CientDLLAddress;
		status = STATUS_SUCCESS;
		byteIO = sizeof(*outPut);
	}
	else if (controlCode == IO_READ_REQUEST)
	{
		DbgPrintEx(0, 0, "收到read信号\n");
		PKERNEL_READ_REQUEST readInput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS gameProcess;
		if (NT_SUCCESS(PsLookupProcessByProcessId(readInput->processId, &gameProcess)))
		{
			KernelReadVirtualMemory(gameProcess, readInput->address, readInput->pBuff, readInput->size);
			status = STATUS_SUCCESS;
			byteIO = sizeof(KERNEL_READ_REQUEST);
		}
	}
	else if (controlCode == IO_WRITE_REQUEST)
	{
		DbgPrintEx(0, 0, "收到write信号\n");
		PKERNEL_WRITE_REQUEST writeInput = (PKERNEL_WRITE_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS gameProcess;
		if (NT_SUCCESS(PsLookupProcessByProcessId(writeInput->processId, &gameProcess)))
		{
			KernelWriteVirtualMemory(gameProcess, writeInput->pBuff, writeInput->address, writeInput->size);
			status = STATUS_SUCCESS;
			byteIO = sizeof(KERNEL_WRITE_REQUEST);
		}
	}

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = byteIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}
