#pragma once

#include <ntifs.h>
#include <ntddk.h>

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT pDriverObject, _In_ PUNICODE_STRING pRrgister);
NTSTATUS UnloadDriver(_In_ PDRIVER_OBJECT pDriverObject);

