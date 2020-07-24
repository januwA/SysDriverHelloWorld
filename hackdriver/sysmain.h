#pragma once

#include <ntifs.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRrgister);

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject);
