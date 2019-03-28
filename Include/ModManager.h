#ifndef MODMANAGER_H_
#define MODMANAGER_H_

typedef int(*ModInitFunc)(void* args);
typedef int(*ModDeInitFunc)();

typedef enum _MOD_ID
{
	MOD_TIMER = 0,
	MOD_LED,
	MOD_BEEP,
	MOD_UART,
	MOD_LCD,
	MOD_NAND,
	MOD_I2C,
	MOD_BUTTON,
	MOD_SUM,
} MOD_ID;

typedef struct _Module
{
	char* modName;
	MOD_ID modId;
	char modIsInit;
	void* modData;
	void* modOps;
} Module;

typedef void (*ModForEachCallback)(Module* pModule);

#define STRUCT_SECTION __attribute__ ((unused, section (".Module")))

#define MODULE_INSTALL(mod, modId, modData, modOperations) Module Module##mod \
	STRUCT_SECTION = {#mod, modId, 0, modData , modOperations};

	
int gLocateProtocol(MOD_ID modId, void** ops);

int gModuleForEach(ModForEachCallback callback);
	
#endif