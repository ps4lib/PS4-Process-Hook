#include "ps4.h"
#include "kern.h"
#include "proc.h"

int gamePID;

int(*sceSysUtilSendSystemNotificationWithText)(int messageType, char* message);

void sysNotify(char* msg) {
	sceSysUtilSendSystemNotificationWithText(222, msg);
}

int _main(void) {
	initKernel();
	initLibc();
	
	kexec(kernelPayload, NULL);
	
	int sysUtil = sceKernelLoadStartModule("/system/common/lib/libSceSysUtil.sprx", 0, NULL, 0, 0, 0);
	RESOLVE(sysUtil, sceSysUtilSendSystemNotificationWithText);
	
	sysNotify("Payload loaded.\nPlease launch Uncharted 4.");
	gamePID = findProcess("eboot.bin");
	
	sceKernelSleep(3);
	
	procAttach(gamePID);
    
	char byte = 1;
	procWriteBytes(gamePID, (void*)0x4FC2E95, &byte, sizeof(byte));
	
	procDetach(gamePID);

	sysNotify("Developer menu enabled!");

	return 0;
}