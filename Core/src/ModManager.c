#include "ModManager.h"

extern int Module_begin;
extern int Module_end;

int gLocateProtocol(MOD_ID modId, void** ops)
{
	Module* pModule = (Module*)&Module_begin;

	if(ops != 0)
	{
		for(pModule = (Module*)&Module_begin; pModule < (Module*)&Module_end; pModule++)
		{
			if(modId == pModule->modId)
			{
				*ops = pModule->modOps;
				
				break;
			}
		}
	}
	return 0;
}

int gModuleForEach(ModForEachCallback callback)
{
	Module* pModule = (Module*)&Module_begin;

	if(callback != 0)
	{
		for(pModule = (Module*)&Module_begin; pModule < (Module*)&Module_end; pModule++)
		{
			callback(pModule);
		}
	}
	return 0;
}