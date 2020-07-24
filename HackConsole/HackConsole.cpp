#include <iostream>
#include "kernelinterface.h"

int main()
{
	KernelInterface driver = KernelInterface("\\\\.\\hackdriver");
	ULONG address = driver.getClientAddress();
	printf_s("Console hDriver: %x \n", driver.hDriver);
	printf_s("Console ClientDLLAddress: %x \n", address);

	while (true)
	{

	}
	return 0;
}
