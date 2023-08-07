#include <fltKernel.h>
#include "../include/oxygenPdb.h"
#define printk(...)do{DbgPrintEx(77,0,__VA_ARGS__);}while(0)


EXTERN_C auto DriverEntry(PDRIVER_OBJECT drv, PUNICODE_STRING) {

	drv->DriverUnload = [](PDRIVER_OBJECT)->void {

	};

	oxygenPdb::Pdber ntos(L"ntoskrnl.exe");
	ntos.init();

	auto p=ntos.GetPointer("ZwCreateThread");
	auto p2 = ntos.GetPointer("PsActiveProcessHead");
	auto o = ntos.GetOffset("_KTHREAD", "PreviousMode");

	printk("p->%p p2->%p o->%x\r\n", p, p2, o);
	return STATUS_SUCCESS;
}