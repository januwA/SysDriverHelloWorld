#pragma once

#include <ntddk.h>

NTKERNELAPI
NTSTATUS
MmCopyVirtualMemory(
	_In_ PEPROCESS SourceProcess,
	_In_ PVOID SourceAddress,

	_In_ PEPROCESS TargetProcess,
	_In_ PVOID TargetAddress,

	_In_ SIZE_T BufferSize,
	_In_	KPROCESSOR_MODE PreviousMode,
	_Out_	PSIZE_T RetureSize
);

NTSTATUS KernelReadVirtualMemory(PEPROCESS process, PVOID sourceAddress, PVOID targetAddress, SIZE_T size);
NTSTATUS KernelWriteVirtualMemory(PEPROCESS process, PVOID sourceAddress, PVOID targetAddress, SIZE_T size);
