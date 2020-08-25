#pragma warning(disable: 4047 4024)
#include "memory.h"

NTSTATUS KernelReadVirtualMemory(PEPROCESS process, PVOID sourceAddress, PVOID targetAddress, SIZE_T size)
{
	PSIZE_T bytes;
	return MmCopyVirtualMemory(process, sourceAddress, PsGetCurrentProcess(), targetAddress, size, KernelMode, &bytes);
}

NTSTATUS KernelWriteVirtualMemory(PEPROCESS process, PVOID sourceAddress, PVOID targetAddress, SIZE_T size)
{
	PSIZE_T bytes;
	return MmCopyVirtualMemory(PsGetCurrentProcess(), sourceAddress, process, targetAddress, size, KernelMode, &bytes);
}
